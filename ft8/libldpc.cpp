///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2023 Edouard Griffiths, F4EXB.                                  //
//                                                                               //
// This is the code from ft8mon: https://github.com/rtmrtmrtmrtm/ft8mon          //
// written by Robert Morris, AB1HL                                               //
// reformatted and adapted to Qt and SDRangel context                            //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

//
// Low Density Parity Check (LDPC) decoder for new FT8.
//
// given a 174-bit codeword as an array of log-likelihood of zero,
// return a 174-bit corrected codeword, or zero-length array.
// first 91 bits are the (systematic) plain-text.
// codeword[i] = log ( P(x=0) / P(x=1) )
//
// this is an implementation of the sum-product algorithm
// from Sarah Johnson's Iterative Error Correction book, and
// Bernhard Leiner's http://www.bernh.net/media/download/papers/ldpc.pdf
//
// cc -O3 libldpc.c -shared -fPIC -o libldpc.so
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "arrays.h"

// float, long float, __float128
#define REAL float

namespace FT8
{
//
// does a 174-bit codeword pass the FT8's LDPC parity checks?
// returns the number of parity checks that passed.
// 83 means total success.
//
int ldpc_check(int codeword[])
{
    int score = 0;

    // Nm[83][7]
    for (int j = 0; j < 83; j++)
    {
        int x = 0;
        for (int ii1 = 0; ii1 < 7; ii1++)
        {
            int i1 = Nm[j][ii1] - 1;
            if (i1 >= 0)
            {
                x ^= codeword[i1];
            }
        }
        if (x == 0)
            score++;
    }
    return score;
}

// llcodeword is 174 log-likelihoods.
// plain is a return value, 174 ints, to be 0 or 1.
// iters is how hard to try.
// ok is the number of parity checks that worked out,
// ok == 83 means success.
void ldpc_decode(float llcodeword[], int iters, int plain[], int *ok)
{
    REAL m[83][174];
    REAL e[83][174];
    REAL codeword[174];
    int best_score = -1;
    int best_cw[174];

    // to translate from log-likelihood x to probability p,
    // p = e**x / (1 + e**x)
    // it's P(zero), not P(one).
    for (int i = 0; i < 174; i++)
    {
        REAL ex = expl(llcodeword[i]);
        REAL p = ex / (1.0 + ex);
        codeword[i] = p;
    }

    // m[j][i] tells the j'th check bit the P(zero) of
    // each of its codeword inputs, based on check
    // bits other than j.
    for (int i = 0; i < 174; i++)
        for (int j = 0; j < 83; j++)
            m[j][i] = codeword[i];

    // e[j][i]: each check j tells each codeword bit i the
    // probability of the bit being zero based
    // on the *other* bits contributing to that check.
    for (int i = 0; i < 174; i++)
        for (int j = 0; j < 83; j++)
            e[j][i] = 0.0;

    for (int iter = 0; iter < iters; iter++)
    {

        for (int j = 0; j < 83; j++)
        {
            for (int ii1 = 0; ii1 < 7; ii1++)
            {
                int i1 = Nm[j][ii1] - 1;
                if (i1 < 0)
                    continue;
                REAL a = 1.0;
                for (int ii2 = 0; ii2 < 7; ii2++)
                {
                    int i2 = Nm[j][ii2] - 1;
                    if (i2 >= 0 && i2 != i1)
                    {
                        // tmp ranges from 1.0 to -1.0, for
                        // definitely zero to definitely one.
                        float tmp = 1.0 - 2.0 * (1.0 - m[j][i2]);
                        a *= tmp;
                    }
                }
                // a ranges from 1.0 to -1.0, meaning
                // bit i1 should be zero .. one.
                // so e[j][i1] will be 0.0 .. 1.0 meaning
                // bit i1 is one .. zero.
                REAL tmp = 0.5 + 0.5 * a;
                e[j][i1] = tmp;
            }
        }

        int cw[174];
        for (int i = 0; i < 174; i++)
        {
            REAL q0 = codeword[i];
            REAL q1 = 1.0 - q0;
            for (int j = 0; j < 3; j++)
            {
                int j2 = Mn[i][j] - 1;
                q0 *= e[j2][i];
                q1 *= 1.0 - e[j2][i];
            }
            // REAL p = q0 / (q0 + q1);
            REAL p;
            if (q0 == 0.0)
            {
                p = 1.0;
            }
            else
            {
                p = 1.0 / (1.0 + (q1 / q0));
            }
            cw[i] = (p <= 0.5);
        }
        int score = ldpc_check(cw);
        if (score == 83)
        {
            for (int i = 0; i < 174; i++)
                plain[i] = cw[i];
            *ok = 83;
            return;
        }

        if (score > best_score)
        {
            for (int i = 0; i < 174; i++)
                best_cw[i] = cw[i];
            best_score = score;
        }

        for (int i = 0; i < 174; i++)
        {
            for (int ji1 = 0; ji1 < 3; ji1++)
            {
                int j1 = Mn[i][ji1] - 1;
                REAL q0 = codeword[i];
                REAL q1 = 1.0 - q0;
                for (int ji2 = 0; ji2 < 3; ji2++)
                {
                    int j2 = Mn[i][ji2] - 1;
                    if (j1 != j2)
                    {
                        q0 *= e[j2][i];
                        q1 *= 1.0 - e[j2][i];
                    }
                }
                // REAL p = q0 / (q0 + q1);
                REAL p;
                if (q0 == 0.0)
                {
                    p = 1.0;
                }
                else
                {
                    p = 1.0 / (1.0 + (q1 / q0));
                }
                m[j1][i] = p;
            }
        }
    }

    // decode didn't work, return best guess.
    for (int i = 0; i < 174; i++)
        plain[i] = best_cw[i];

    *ok = best_score;
}

// thank you Douglas Bagnall
// https://math.stackexchange.com/a/446411
float fast_tanh(float x)
{
    if (x < -7.6)
    {
        return -0.999;
    }
    if (x > 7.6)
    {
        return 0.999;
    }
    float x2 = x * x;
    float a = x * (135135.0f + x2 * (17325.0f + x2 * (378.0f + x2)));
    float b = 135135.0f + x2 * (62370.0f + x2 * (3150.0f + x2 * 28.0f));
    return a / b;
}

#if 0
#define TANGRAN 0.01
static float tanhtable[];

float
table_tanh(float x)
{
int ind = (x - (-5.0)) / TANGRAN;
if(ind < 0){
return -1.0;
}
if(ind >= 1000){
return 1.0;
}
return tanhtable[ind];
}
#endif

// codeword is 174 log-likelihoods.
// plain is a return value, 174 ints, to be 0 or 1.
// iters is how hard to try.
// ok is the number of parity checks that worked out,
// ok == 83 means success.
void ldpc_decode_log(float codeword[], int iters, int plain[], int *ok)
{
    REAL m[83][174];
    REAL e[83][174];
    int best_score = -1;
    int best_cw[174];

    for (int i = 0; i < 174; i++)
        for (int j = 0; j < 83; j++)
            m[j][i] = codeword[i];

    for (int i = 0; i < 174; i++)
        for (int j = 0; j < 83; j++)
            e[j][i] = 0.0;

    for (int iter = 0; iter < iters; iter++)
    {
        for (int j = 0; j < 83; j++)
        {
            for (int ii1 = 0; ii1 < 7; ii1++)
            {
                int i1 = Nm[j][ii1] - 1;
                if (i1 < 0)
                    continue;
                REAL a = 1.0;
                for (int ii2 = 0; ii2 < 7; ii2++)
                {
                    int i2 = Nm[j][ii2] - 1;
                    if (i2 >= 0 && i2 != i1)
                    {
                        // a *= table_tanh(m[j][i2] / 2.0);
                        a *= fast_tanh(m[j][i2] / 2.0);
                    }
                }
                REAL tmp;
                if (a >= 0.999)
                {
                    tmp = 7.6;
                }
                else if (a <= -0.999)
                {
                    tmp = -7.6;
                }
                else
                {
                    tmp = log((1 + a) / (1 - a));
                }
                e[j][i1] = tmp;
            }
        }

        int cw[174];
        for (int i = 0; i < 174; i++)
        {
            REAL l = codeword[i];
            for (int j = 0; j < 3; j++)
                l += e[Mn[i][j] - 1][i];
            cw[i] = (l <= 0.0);
        }
        int score = ldpc_check(cw);
        if (score == 83)
        {
            for (int i = 0; i < 174; i++)
                plain[i] = cw[i];
            *ok = 83;
            return;
        }

        if (score > best_score)
        {
            for (int i = 0; i < 174; i++)
                best_cw[i] = cw[i];
            best_score = score;
        }

        for (int i = 0; i < 174; i++)
        {
            for (int ji1 = 0; ji1 < 3; ji1++)
            {
                int j1 = Mn[i][ji1] - 1;
                REAL l = codeword[i];
                for (int ji2 = 0; ji2 < 3; ji2++)
                {
                    int j2 = Mn[i][ji2] - 1;
                    if (j1 != j2)
                    {
                        l += e[j2][i];
                    }
                }
                m[j1][i] = l;
            }
        }
    }

    // decode didn't work, return best guess.
    for (int i = 0; i < 174; i++)
        plain[i] = best_cw[i];

    *ok = best_score;
}

//
// check the FT8 CRC-14
//

void ft8_crc(int msg1[], int msglen, int out[14])
{
    // the old FT8 polynomial for 12-bit CRC, 0xc06.
    // int div[] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 };

    // the new FT8 polynomial for 14-bit CRC, 0x2757,
    // with leading 1 bit.
    int div[] = {1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};

    // append 14 zeros.
    int *msg = (int *)malloc(sizeof(int) * (msglen + 14));
    for (int i = 0; i < msglen + 14; i++)
    {
        if (i < msglen)
        {
            msg[i] = msg1[i];
        }
        else
        {
            msg[i] = 0;
        }
    }

    for (int i = 0; i < msglen; i++)
    {
        if (msg[i])
        {
            for (int j = 0; j < 15; j++)
            {
                msg[i + j] = (msg[i + j] + div[j]) % 2;
            }
        }
    }

    for (int i = 0; i < 14; i++)
    {
        out[i] = msg[msglen + i];
    }

    free(msg);
}

// rows is 91, cols is 174.
// m[174][2*91].
// m's right half should start out as zeros.
// m's upper-right quarter will be the desired inverse.
void gauss_jordan(int rows, int cols, int m[174][2 * 91], int which[91], int *ok)
// gauss_jordan(int rows, int cols, int m[cols][2*rows], int which[rows], int *ok)
{
    *ok = 0;

    assert(rows == 91);
    assert(cols == 174);

    for (int row = 0; row < rows; row++)
    {
        if (m[row][row] != 1)
        {
            for (int row1 = row + 1; row1 < cols; row1++)
            {
                if (m[row1][row] == 1)
                {
                    // swap m[row] and m[row1]
                    for (int col = 0; col < 2 * rows; col++)
                    {
                        int tmp = m[row][col];
                        m[row][col] = m[row1][col];
                        m[row1][col] = tmp;
                    }
                    int tmp = which[row];
                    which[row] = which[row1];
                    which[row1] = tmp;
                    break;
                }
            }
        }
        if (m[row][row] != 1)
        {
            // could not invert
            *ok = 0;
            return;
        }
        // lazy creation of identity matrix in the upper-right quarter
        m[row][rows + row] = (m[row][rows + row] + 1) % 2;
        // now eliminate
        for (int row1 = 0; row1 < cols; row1++)
        {
            if (row1 == row)
                continue;
            if (m[row1][row] != 0)
            {
                for (int col = 0; col < 2 * rows; col++)
                {
                    m[row1][col] = (m[row1][col] + m[row][col]) % 2;
                }
            }
        }
    }

    *ok = 1;
}

//  # given a 174-bit codeword as an array of log-likelihood of zero,
//  # return a 87-bit plain text, or zero-length array.
//  # this is an implementation of the sum-product algorithm
//  # from Sarah Johnson's Iterative Error Correction book.
//  # codeword[i] = log ( P(x=0) / P(x=1) )
//  def ldpc_decode(self, codeword):
//      # 174 codeword bits
//      # 87 parity checks
//
//      # Mji
//      # each codeword bit i tells each parity check j
//      # what the bit's log-likelihood of being 0 is
//      # based on information *other* than from that
//      # parity check.
//      m = numpy.zeros((87, 174))
//
//      # Eji
//      # each check j tells each codeword bit i the
//      # log likelihood of the bit being zero based
//      # on the *other* bits in that check.
//      e = numpy.zeros((87, 174))
//
//      for i in range(0, 174):
//          for j in range(0, 87):
//              m[j][i] = codeword[i]
//
//      for iter in range(0, 50):
//          # messages from checks to bits.
//          # for each parity check
//          for j in range(0, 87):
//              # for each bit mentioned in this parity check
//              for i in Nm[j]:
//                  if i <= 0:
//                      continue
//                  a = 1
//                  # for each other bit mentioned in this parity check
//                  for ii in Nm[j]:
//                      if ii != i:
//                          a *= math.tanh(m[j][ii-1] / 2.0)
//                  e[j][i-1] = math.log((1 + a) / (1 - a))
//
//          # decide if we are done -- compute the corrected codeword,
//          # see if the parity check succeeds.
//          cw = numpy.zeros(174, dtype=numpy.int32)
//          for i in range(0, 174):
//              # sum the log likelihoods for codeword bit i being 0.
//              l = codeword[i]
//              for j in Mn[i]:
//                  l += e[j-1][i]
//              if l > 0:
//                  cw[i] = 0
//              else:
//                  cw[i] = 1
//          if self.ldpc_check(cw):
//              # success!
//              # it's a systematic code, though the plain-text bits are scattered.
//              # collect them.
//              decoded = cw[colorder]
//              decoded = decoded[-87:]
//              return decoded
//
//          # messages from bits to checks.
//          for i in range(0, 174):
//              for j in Mn[i]:
//                  l = codeword[i]
//                  for jj in Mn[i]:
//                      if jj != j:
//                          l += e[jj-1][i]
//                  m[j-1][i] = l
//
//      # could not decode.
//      return numpy.array([])

#if 0
static float tanhtable[] = {
-0.99990920, -0.99990737, -0.99990550, -0.99990359, -0.99990164,
 -0.99989966, -0.99989763, -0.99989556, -0.99989345, -0.99989130,
 -0.99988910, -0.99988686, -0.99988458, -0.99988225, -0.99987987,
 -0.99987744, -0.99987496, -0.99987244, -0.99986986, -0.99986723,
 -0.99986455, -0.99986182, -0.99985902, -0.99985618, -0.99985327,
 -0.99985031, -0.99984728, -0.99984420, -0.99984105, -0.99983784,
 -0.99983457, -0.99983122, -0.99982781, -0.99982434, -0.99982079,
 -0.99981717, -0.99981348, -0.99980971, -0.99980586, -0.99980194,
 -0.99979794, -0.99979386, -0.99978970, -0.99978545, -0.99978111,
 -0.99977669, -0.99977218, -0.99976758, -0.99976289, -0.99975810,
 -0.99975321, -0.99974823, -0.99974314, -0.99973795, -0.99973266,
 -0.99972726, -0.99972175, -0.99971613, -0.99971040, -0.99970455,
 -0.99969858, -0.99969249, -0.99968628, -0.99967994, -0.99967348,
 -0.99966688, -0.99966016, -0.99965329, -0.99964629, -0.99963914,
 -0.99963186, -0.99962442, -0.99961683, -0.99960910, -0.99960120,
 -0.99959315, -0.99958493, -0.99957655, -0.99956799, -0.99955927,
 -0.99955037, -0.99954129, -0.99953202, -0.99952257, -0.99951293,
 -0.99950309, -0.99949305, -0.99948282, -0.99947237, -0.99946171,
 -0.99945084, -0.99943975, -0.99942844, -0.99941690, -0.99940512,
 -0.99939311, -0.99938085, -0.99936835, -0.99935559, -0.99934258,
 -0.99932930, -0.99931576, -0.99930194, -0.99928784, -0.99927346,
 -0.99925879, -0.99924382, -0.99922855, -0.99921297, -0.99919708,
 -0.99918087, -0.99916432, -0.99914745, -0.99913024, -0.99911267,
 -0.99909476, -0.99907648, -0.99905783, -0.99903881, -0.99901940,
 -0.99899960, -0.99897940, -0.99895879, -0.99893777, -0.99891632,
 -0.99889444, -0.99887212, -0.99884935, -0.99882612, -0.99880242,
 -0.99877824, -0.99875358, -0.99872841, -0.99870274, -0.99867655,
 -0.99864983, -0.99862258, -0.99859477, -0.99856640, -0.99853747,
 -0.99850794, -0.99847782, -0.99844710, -0.99841575, -0.99838377,
 -0.99835115, -0.99831787, -0.99828392, -0.99824928, -0.99821395,
 -0.99817790, -0.99814112, -0.99810361, -0.99806533, -0.99802629,
 -0.99798646, -0.99794582, -0.99790437, -0.99786208, -0.99781894,
 -0.99777493, -0.99773003, -0.99768423, -0.99763750, -0.99758983,
 -0.99754120, -0.99749159, -0.99744099, -0.99738936, -0.99733669,
 -0.99728296, -0.99722815, -0.99717223, -0.99711519, -0.99705700,
 -0.99699764, -0.99693708, -0.99687530, -0.99681228, -0.99674798,
 -0.99668240, -0.99661549, -0.99654724, -0.99647761, -0.99640658,
 -0.99633412, -0.99626020, -0.99618480, -0.99610788, -0.99602941,
 -0.99594936, -0.99586770, -0.99578440, -0.99569942, -0.99561273,
 -0.99552430, -0.99543409, -0.99534207, -0.99524820, -0.99515244,
 -0.99505475, -0.99495511, -0.99485345, -0.99474976, -0.99464398,
 -0.99453608, -0.99442601, -0.99431373, -0.99419919, -0.99408235,
 -0.99396317, -0.99384159, -0.99371757, -0.99359107, -0.99346202,
 -0.99333039, -0.99319611, -0.99305914, -0.99291942, -0.99277690,
 -0.99263152, -0.99248323, -0.99233196, -0.99217766, -0.99202027,
 -0.99185972, -0.99169596, -0.99152892, -0.99135853, -0.99118473,
 -0.99100745, -0.99082663, -0.99064218, -0.99045404, -0.99026214,
 -0.99006640, -0.98986674, -0.98966309, -0.98945538, -0.98924351,
 -0.98902740, -0.98880698, -0.98858216, -0.98835285, -0.98811896,
 -0.98788040, -0.98763708, -0.98738891, -0.98713578, -0.98687761,
 -0.98661430, -0.98634574, -0.98607182, -0.98579245, -0.98550752,
 -0.98521692, -0.98492053, -0.98461825, -0.98430995, -0.98399553,
 -0.98367486, -0.98334781, -0.98301427, -0.98267411, -0.98232720,
 -0.98197340, -0.98161259, -0.98124462, -0.98086936, -0.98048667,
 -0.98009640, -0.97969840, -0.97929252, -0.97887862, -0.97845654,
 -0.97802611, -0.97758719, -0.97713959, -0.97668317, -0.97621774,
 -0.97574313, -0.97525917, -0.97476568, -0.97426247, -0.97374936,
 -0.97322616, -0.97269268, -0.97214872, -0.97159408, -0.97102855,
 -0.97045194, -0.96986402, -0.96926459, -0.96865342, -0.96803030,
 -0.96739500, -0.96674729, -0.96608693, -0.96541369, -0.96472732,
 -0.96402758, -0.96331422, -0.96258698, -0.96184561, -0.96108983,
 -0.96031939, -0.95953401, -0.95873341, -0.95791731, -0.95708542,
 -0.95623746, -0.95537312, -0.95449211, -0.95359412, -0.95267884,
 -0.95174596, -0.95079514, -0.94982608, -0.94883842, -0.94783185,
 -0.94680601, -0.94576057, -0.94469516, -0.94360942, -0.94250301,
 -0.94137554, -0.94022664, -0.93905593, -0.93786303, -0.93664754,
 -0.93540907, -0.93414721, -0.93286155, -0.93155168, -0.93021718,
 -0.92885762, -0.92747257, -0.92606158, -0.92462422, -0.92316003,
 -0.92166855, -0.92014933, -0.91860189, -0.91702576, -0.91542046,
 -0.91378549, -0.91212037, -0.91042459, -0.90869766, -0.90693905,
 -0.90514825, -0.90332474, -0.90146799, -0.89957745, -0.89765260,
 -0.89569287, -0.89369773, -0.89166660, -0.88959892, -0.88749413,
 -0.88535165, -0.88317089, -0.88095127, -0.87869219, -0.87639307,
 -0.87405329, -0.87167225, -0.86924933, -0.86678393, -0.86427541,
 -0.86172316, -0.85912654, -0.85648492, -0.85379765, -0.85106411,
 -0.84828364, -0.84545560, -0.84257933, -0.83965418, -0.83667949,
 -0.83365461, -0.83057887, -0.82745161, -0.82427217, -0.82103988,
 -0.81775408, -0.81441409, -0.81101926, -0.80756892, -0.80406239,
 -0.80049902, -0.79687814, -0.79319910, -0.78946122, -0.78566386,
 -0.78180636, -0.77788807, -0.77390834, -0.76986654, -0.76576202,
 -0.76159416, -0.75736232, -0.75306590, -0.74870429, -0.74427687,
 -0.73978305, -0.73522225, -0.73059390, -0.72589741, -0.72113225,
 -0.71629787, -0.71139373, -0.70641932, -0.70137413, -0.69625767,
 -0.69106947, -0.68580906, -0.68047601, -0.67506987, -0.66959026,
 -0.66403677, -0.65840904, -0.65270671, -0.64692945, -0.64107696,
 -0.63514895, -0.62914516, -0.62306535, -0.61690930, -0.61067683,
 -0.60436778, -0.59798200, -0.59151940, -0.58497988, -0.57836341,
 -0.57166997, -0.56489955, -0.55805222, -0.55112803, -0.54412710,
 -0.53704957, -0.52989561, -0.52266543, -0.51535928, -0.50797743,
 -0.50052021, -0.49298797, -0.48538109, -0.47770001, -0.46994520,
 -0.46211716, -0.45421643, -0.44624361, -0.43819931, -0.43008421,
 -0.42189901, -0.41364444, -0.40532131, -0.39693043, -0.38847268,
 -0.37994896, -0.37136023, -0.36270747, -0.35399171, -0.34521403,
 -0.33637554, -0.32747739, -0.31852078, -0.30950692, -0.30043710,
 -0.29131261, -0.28213481, -0.27290508, -0.26362484, -0.25429553,
 -0.24491866, -0.23549575, -0.22602835, -0.21651806, -0.20696650,
 -0.19737532, -0.18774621, -0.17808087, -0.16838105, -0.15864850,
 -0.14888503, -0.13909245, -0.12927258, -0.11942730, -0.10955847,
 -0.09966799, -0.08975778, -0.07982977, -0.06988589, -0.05992810,
 -0.04995837, -0.03997868, -0.02999100, -0.01999733, -0.00999967,
 -0.00000000, 0.00999967, 0.01999733, 0.02999100, 0.03997868,
 0.04995837, 0.05992810, 0.06988589, 0.07982977, 0.08975778,
 0.09966799, 0.10955847, 0.11942730, 0.12927258, 0.13909245,
 0.14888503, 0.15864850, 0.16838105, 0.17808087, 0.18774621,
 0.19737532, 0.20696650, 0.21651806, 0.22602835, 0.23549575,
 0.24491866, 0.25429553, 0.26362484, 0.27290508, 0.28213481,
 0.29131261, 0.30043710, 0.30950692, 0.31852078, 0.32747739,
 0.33637554, 0.34521403, 0.35399171, 0.36270747, 0.37136023,
 0.37994896, 0.38847268, 0.39693043, 0.40532131, 0.41364444,
 0.42189901, 0.43008421, 0.43819931, 0.44624361, 0.45421643,
 0.46211716, 0.46994520, 0.47770001, 0.48538109, 0.49298797,
 0.50052021, 0.50797743, 0.51535928, 0.52266543, 0.52989561,
 0.53704957, 0.54412710, 0.55112803, 0.55805222, 0.56489955,
 0.57166997, 0.57836341, 0.58497988, 0.59151940, 0.59798200,
 0.60436778, 0.61067683, 0.61690930, 0.62306535, 0.62914516,
 0.63514895, 0.64107696, 0.64692945, 0.65270671, 0.65840904,
 0.66403677, 0.66959026, 0.67506987, 0.68047601, 0.68580906,
 0.69106947, 0.69625767, 0.70137413, 0.70641932, 0.71139373,
 0.71629787, 0.72113225, 0.72589741, 0.73059390, 0.73522225,
 0.73978305, 0.74427687, 0.74870429, 0.75306590, 0.75736232,
 0.76159416, 0.76576202, 0.76986654, 0.77390834, 0.77788807,
 0.78180636, 0.78566386, 0.78946122, 0.79319910, 0.79687814,
 0.80049902, 0.80406239, 0.80756892, 0.81101926, 0.81441409,
 0.81775408, 0.82103988, 0.82427217, 0.82745161, 0.83057887,
 0.83365461, 0.83667949, 0.83965418, 0.84257933, 0.84545560,
 0.84828364, 0.85106411, 0.85379765, 0.85648492, 0.85912654,
 0.86172316, 0.86427541, 0.86678393, 0.86924933, 0.87167225,
 0.87405329, 0.87639307, 0.87869219, 0.88095127, 0.88317089,
 0.88535165, 0.88749413, 0.88959892, 0.89166660, 0.89369773,
 0.89569287, 0.89765260, 0.89957745, 0.90146799, 0.90332474,
 0.90514825, 0.90693905, 0.90869766, 0.91042459, 0.91212037,
 0.91378549, 0.91542046, 0.91702576, 0.91860189, 0.92014933,
 0.92166855, 0.92316003, 0.92462422, 0.92606158, 0.92747257,
 0.92885762, 0.93021718, 0.93155168, 0.93286155, 0.93414721,
 0.93540907, 0.93664754, 0.93786303, 0.93905593, 0.94022664,
 0.94137554, 0.94250301, 0.94360942, 0.94469516, 0.94576057,
 0.94680601, 0.94783185, 0.94883842, 0.94982608, 0.95079514,
 0.95174596, 0.95267884, 0.95359412, 0.95449211, 0.95537312,
 0.95623746, 0.95708542, 0.95791731, 0.95873341, 0.95953401,
 0.96031939, 0.96108983, 0.96184561, 0.96258698, 0.96331422,
 0.96402758, 0.96472732, 0.96541369, 0.96608693, 0.96674729,
 0.96739500, 0.96803030, 0.96865342, 0.96926459, 0.96986402,
 0.97045194, 0.97102855, 0.97159408, 0.97214872, 0.97269268,
 0.97322616, 0.97374936, 0.97426247, 0.97476568, 0.97525917,
 0.97574313, 0.97621774, 0.97668317, 0.97713959, 0.97758719,
 0.97802611, 0.97845654, 0.97887862, 0.97929252, 0.97969840,
 0.98009640, 0.98048667, 0.98086936, 0.98124462, 0.98161259,
 0.98197340, 0.98232720, 0.98267411, 0.98301427, 0.98334781,
 0.98367486, 0.98399553, 0.98430995, 0.98461825, 0.98492053,
 0.98521692, 0.98550752, 0.98579245, 0.98607182, 0.98634574,
 0.98661430, 0.98687761, 0.98713578, 0.98738891, 0.98763708,
 0.98788040, 0.98811896, 0.98835285, 0.98858216, 0.98880698,
 0.98902740, 0.98924351, 0.98945538, 0.98966309, 0.98986674,
 0.99006640, 0.99026214, 0.99045404, 0.99064218, 0.99082663,
 0.99100745, 0.99118473, 0.99135853, 0.99152892, 0.99169596,
 0.99185972, 0.99202027, 0.99217766, 0.99233196, 0.99248323,
 0.99263152, 0.99277690, 0.99291942, 0.99305914, 0.99319611,
 0.99333039, 0.99346202, 0.99359107, 0.99371757, 0.99384159,
 0.99396317, 0.99408235, 0.99419919, 0.99431373, 0.99442601,
 0.99453608, 0.99464398, 0.99474976, 0.99485345, 0.99495511,
 0.99505475, 0.99515244, 0.99524820, 0.99534207, 0.99543409,
 0.99552430, 0.99561273, 0.99569942, 0.99578440, 0.99586770,
 0.99594936, 0.99602941, 0.99610788, 0.99618480, 0.99626020,
 0.99633412, 0.99640658, 0.99647761, 0.99654724, 0.99661549,
 0.99668240, 0.99674798, 0.99681228, 0.99687530, 0.99693708,
 0.99699764, 0.99705700, 0.99711519, 0.99717223, 0.99722815,
 0.99728296, 0.99733669, 0.99738936, 0.99744099, 0.99749159,
 0.99754120, 0.99758983, 0.99763750, 0.99768423, 0.99773003,
 0.99777493, 0.99781894, 0.99786208, 0.99790437, 0.99794582,
 0.99798646, 0.99802629, 0.99806533, 0.99810361, 0.99814112,
 0.99817790, 0.99821395, 0.99824928, 0.99828392, 0.99831787,
 0.99835115, 0.99838377, 0.99841575, 0.99844710, 0.99847782,
 0.99850794, 0.99853747, 0.99856640, 0.99859477, 0.99862258,
 0.99864983, 0.99867655, 0.99870274, 0.99872841, 0.99875358,
 0.99877824, 0.99880242, 0.99882612, 0.99884935, 0.99887212,
 0.99889444, 0.99891632, 0.99893777, 0.99895879, 0.99897940,
 0.99899960, 0.99901940, 0.99903881, 0.99905783, 0.99907648,
 0.99909476, 0.99911267, 0.99913024, 0.99914745, 0.99916432,
 0.99918087, 0.99919708, 0.99921297, 0.99922855, 0.99924382,
 0.99925879, 0.99927346, 0.99928784, 0.99930194, 0.99931576,
 0.99932930, 0.99934258, 0.99935559, 0.99936835, 0.99938085,
 0.99939311, 0.99940512, 0.99941690, 0.99942844, 0.99943975,
 0.99945084, 0.99946171, 0.99947237, 0.99948282, 0.99949305,
 0.99950309, 0.99951293, 0.99952257, 0.99953202, 0.99954129,
 0.99955037, 0.99955927, 0.99956799, 0.99957655, 0.99958493,
 0.99959315, 0.99960120, 0.99960910, 0.99961683, 0.99962442,
 0.99963186, 0.99963914, 0.99964629, 0.99965329, 0.99966016,
 0.99966688, 0.99967348, 0.99967994, 0.99968628, 0.99969249,
 0.99969858, 0.99970455, 0.99971040, 0.99971613, 0.99972175,
 0.99972726, 0.99973266, 0.99973795, 0.99974314, 0.99974823,
 0.99975321, 0.99975810, 0.99976289, 0.99976758, 0.99977218,
 0.99977669, 0.99978111, 0.99978545, 0.99978970, 0.99979386,
 0.99979794, 0.99980194, 0.99980586, 0.99980971, 0.99981348,
 0.99981717, 0.99982079, 0.99982434, 0.99982781, 0.99983122,
 0.99983457, 0.99983784, 0.99984105, 0.99984420, 0.99984728,
 0.99985031, 0.99985327, 0.99985618, 0.99985902, 0.99986182,
 0.99986455, 0.99986723, 0.99986986, 0.99987244, 0.99987496,
 0.99987744, 0.99987987, 0.99988225, 0.99988458, 0.99988686,
 0.99988910, 0.99989130, 0.99989345, 0.99989556, 0.99989763,
 0.99989966, 0.99990164, 0.99990359, 0.99990550, 0.99990737,
 0.99990920,
};
#endif

} // namespace FT8
