/**
 * SDRangel
 * This is the web REST/JSON API of SDRangel SDR software. SDRangel is an Open Source Qt5/OpenGL 3.0+ (4.3+ in Windows) GUI and server Software Defined Radio and signal analyzer in software. It supports Airspy, BladeRF, HackRF, LimeSDR, PlutoSDR, RTL-SDR, SDRplay RSP1 and FunCube    ---   Limitations and specifcities:    * In SDRangel GUI the first Rx device set cannot be deleted. Conversely the server starts with no device sets and its number of device sets can be reduced to zero by as many calls as necessary to /sdrangel/deviceset with DELETE method.   * Preset import and export from/to file is a server only feature.   * Device set focus is a GUI only feature.   * The following channels are not implemented (status 501 is returned): ATV and DATV demodulators, Channel Analyzer NG, LoRa demodulator   * The device settings and report structures contains only the sub-structure corresponding to the device type. The DeviceSettings and DeviceReport structures documented here shows all of them but only one will be or should be present at a time   * The channel settings and report structures contains only the sub-structure corresponding to the channel type. The ChannelSettings and ChannelReport structures documented here shows all of them but only one will be or should be present at a time    --- 
 *
 * OpenAPI spec version: 6.0.0
 * Contact: f4exb06@gmail.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */


#include "SWGChannelAnalyzerSettings.h"

#include "SWGHelpers.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QDebug>

namespace SWGSDRangel {

SWGChannelAnalyzerSettings::SWGChannelAnalyzerSettings(QString* json) {
    init();
    this->fromJson(*json);
}

SWGChannelAnalyzerSettings::SWGChannelAnalyzerSettings() {
    frequency = 0;
    m_frequency_isSet = false;
    down_sample = 0;
    m_down_sample_isSet = false;
    down_sample_rate = 0;
    m_down_sample_rate_isSet = false;
    bandwidth = 0;
    m_bandwidth_isSet = false;
    low_cutoff = 0;
    m_low_cutoff_isSet = false;
    span_log2 = 0;
    m_span_log2_isSet = false;
    ssb = 0;
    m_ssb_isSet = false;
    pll = 0;
    m_pll_isSet = false;
    fll = 0;
    m_fll_isSet = false;
    costas_loop = 0;
    m_costas_loop_isSet = false;
    rrc = 0;
    m_rrc_isSet = false;
    rrc_rolloff = 0;
    m_rrc_rolloff_isSet = false;
    pll_psk_order = 0;
    m_pll_psk_order_isSet = false;
    pll_bandwidth = 0.0f;
    m_pll_bandwidth_isSet = false;
    pll_damping_factor = 0.0f;
    m_pll_damping_factor_isSet = false;
    pll_loop_gain = 0.0f;
    m_pll_loop_gain_isSet = false;
    input_type = 0;
    m_input_type_isSet = false;
    rgb_color = 0;
    m_rgb_color_isSet = false;
    title = nullptr;
    m_title_isSet = false;
    stream_index = 0;
    m_stream_index_isSet = false;
    use_reverse_api = 0;
    m_use_reverse_api_isSet = false;
    reverse_api_address = nullptr;
    m_reverse_api_address_isSet = false;
    reverse_api_port = 0;
    m_reverse_api_port_isSet = false;
    reverse_api_device_index = 0;
    m_reverse_api_device_index_isSet = false;
    reverse_api_channel_index = 0;
    m_reverse_api_channel_index_isSet = false;
    spectrum_config = nullptr;
    m_spectrum_config_isSet = false;
    scope_config = nullptr;
    m_scope_config_isSet = false;
    channel_marker = nullptr;
    m_channel_marker_isSet = false;
}

SWGChannelAnalyzerSettings::~SWGChannelAnalyzerSettings() {
    this->cleanup();
}

void
SWGChannelAnalyzerSettings::init() {
    frequency = 0;
    m_frequency_isSet = false;
    down_sample = 0;
    m_down_sample_isSet = false;
    down_sample_rate = 0;
    m_down_sample_rate_isSet = false;
    bandwidth = 0;
    m_bandwidth_isSet = false;
    low_cutoff = 0;
    m_low_cutoff_isSet = false;
    span_log2 = 0;
    m_span_log2_isSet = false;
    ssb = 0;
    m_ssb_isSet = false;
    pll = 0;
    m_pll_isSet = false;
    fll = 0;
    m_fll_isSet = false;
    costas_loop = 0;
    m_costas_loop_isSet = false;
    rrc = 0;
    m_rrc_isSet = false;
    rrc_rolloff = 0;
    m_rrc_rolloff_isSet = false;
    pll_psk_order = 0;
    m_pll_psk_order_isSet = false;
    pll_bandwidth = 0.0f;
    m_pll_bandwidth_isSet = false;
    pll_damping_factor = 0.0f;
    m_pll_damping_factor_isSet = false;
    pll_loop_gain = 0.0f;
    m_pll_loop_gain_isSet = false;
    input_type = 0;
    m_input_type_isSet = false;
    rgb_color = 0;
    m_rgb_color_isSet = false;
    title = new QString("");
    m_title_isSet = false;
    stream_index = 0;
    m_stream_index_isSet = false;
    use_reverse_api = 0;
    m_use_reverse_api_isSet = false;
    reverse_api_address = new QString("");
    m_reverse_api_address_isSet = false;
    reverse_api_port = 0;
    m_reverse_api_port_isSet = false;
    reverse_api_device_index = 0;
    m_reverse_api_device_index_isSet = false;
    reverse_api_channel_index = 0;
    m_reverse_api_channel_index_isSet = false;
    spectrum_config = new SWGGLSpectrum();
    m_spectrum_config_isSet = false;
    scope_config = new SWGGLScope();
    m_scope_config_isSet = false;
    channel_marker = new SWGChannelMarker();
    m_channel_marker_isSet = false;
}

void
SWGChannelAnalyzerSettings::cleanup() {


















    if(title != nullptr) { 
        delete title;
    }


    if(reverse_api_address != nullptr) { 
        delete reverse_api_address;
    }



    if(spectrum_config != nullptr) { 
        delete spectrum_config;
    }
    if(scope_config != nullptr) { 
        delete scope_config;
    }
    if(channel_marker != nullptr) { 
        delete channel_marker;
    }
}

SWGChannelAnalyzerSettings*
SWGChannelAnalyzerSettings::fromJson(QString &json) {
    QByteArray array (json.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
    return this;
}

void
SWGChannelAnalyzerSettings::fromJsonObject(QJsonObject &pJson) {
    ::SWGSDRangel::setValue(&frequency, pJson["frequency"], "qint32", "");
    
    ::SWGSDRangel::setValue(&down_sample, pJson["downSample"], "qint32", "");
    
    ::SWGSDRangel::setValue(&down_sample_rate, pJson["downSampleRate"], "qint32", "");
    
    ::SWGSDRangel::setValue(&bandwidth, pJson["bandwidth"], "qint32", "");
    
    ::SWGSDRangel::setValue(&low_cutoff, pJson["lowCutoff"], "qint32", "");
    
    ::SWGSDRangel::setValue(&span_log2, pJson["spanLog2"], "qint32", "");
    
    ::SWGSDRangel::setValue(&ssb, pJson["ssb"], "qint32", "");
    
    ::SWGSDRangel::setValue(&pll, pJson["pll"], "qint32", "");
    
    ::SWGSDRangel::setValue(&fll, pJson["fll"], "qint32", "");
    
    ::SWGSDRangel::setValue(&costas_loop, pJson["costasLoop"], "qint32", "");
    
    ::SWGSDRangel::setValue(&rrc, pJson["rrc"], "qint32", "");
    
    ::SWGSDRangel::setValue(&rrc_rolloff, pJson["rrcRolloff"], "qint32", "");
    
    ::SWGSDRangel::setValue(&pll_psk_order, pJson["pllPskOrder"], "qint32", "");
    
    ::SWGSDRangel::setValue(&pll_bandwidth, pJson["pllBandwidth"], "float", "");
    
    ::SWGSDRangel::setValue(&pll_damping_factor, pJson["pllDampingFactor"], "float", "");
    
    ::SWGSDRangel::setValue(&pll_loop_gain, pJson["pllLoopGain"], "float", "");
    
    ::SWGSDRangel::setValue(&input_type, pJson["inputType"], "qint32", "");
    
    ::SWGSDRangel::setValue(&rgb_color, pJson["rgbColor"], "qint32", "");
    
    ::SWGSDRangel::setValue(&title, pJson["title"], "QString", "QString");
    
    ::SWGSDRangel::setValue(&stream_index, pJson["streamIndex"], "qint32", "");
    
    ::SWGSDRangel::setValue(&use_reverse_api, pJson["useReverseAPI"], "qint32", "");
    
    ::SWGSDRangel::setValue(&reverse_api_address, pJson["reverseAPIAddress"], "QString", "QString");
    
    ::SWGSDRangel::setValue(&reverse_api_port, pJson["reverseAPIPort"], "qint32", "");
    
    ::SWGSDRangel::setValue(&reverse_api_device_index, pJson["reverseAPIDeviceIndex"], "qint32", "");
    
    ::SWGSDRangel::setValue(&reverse_api_channel_index, pJson["reverseAPIChannelIndex"], "qint32", "");
    
    ::SWGSDRangel::setValue(&spectrum_config, pJson["spectrumConfig"], "SWGGLSpectrum", "SWGGLSpectrum");
    
    ::SWGSDRangel::setValue(&scope_config, pJson["scopeConfig"], "SWGGLScope", "SWGGLScope");
    
    ::SWGSDRangel::setValue(&channel_marker, pJson["channelMarker"], "SWGChannelMarker", "SWGChannelMarker");
    
}

QString
SWGChannelAnalyzerSettings::asJson ()
{
    QJsonObject* obj = this->asJsonObject();

    QJsonDocument doc(*obj);
    QByteArray bytes = doc.toJson();
    delete obj;
    return QString(bytes);
}

QJsonObject*
SWGChannelAnalyzerSettings::asJsonObject() {
    QJsonObject* obj = new QJsonObject();
    if(m_frequency_isSet){
        obj->insert("frequency", QJsonValue(frequency));
    }
    if(m_down_sample_isSet){
        obj->insert("downSample", QJsonValue(down_sample));
    }
    if(m_down_sample_rate_isSet){
        obj->insert("downSampleRate", QJsonValue(down_sample_rate));
    }
    if(m_bandwidth_isSet){
        obj->insert("bandwidth", QJsonValue(bandwidth));
    }
    if(m_low_cutoff_isSet){
        obj->insert("lowCutoff", QJsonValue(low_cutoff));
    }
    if(m_span_log2_isSet){
        obj->insert("spanLog2", QJsonValue(span_log2));
    }
    if(m_ssb_isSet){
        obj->insert("ssb", QJsonValue(ssb));
    }
    if(m_pll_isSet){
        obj->insert("pll", QJsonValue(pll));
    }
    if(m_fll_isSet){
        obj->insert("fll", QJsonValue(fll));
    }
    if(m_costas_loop_isSet){
        obj->insert("costasLoop", QJsonValue(costas_loop));
    }
    if(m_rrc_isSet){
        obj->insert("rrc", QJsonValue(rrc));
    }
    if(m_rrc_rolloff_isSet){
        obj->insert("rrcRolloff", QJsonValue(rrc_rolloff));
    }
    if(m_pll_psk_order_isSet){
        obj->insert("pllPskOrder", QJsonValue(pll_psk_order));
    }
    if(m_pll_bandwidth_isSet){
        obj->insert("pllBandwidth", QJsonValue(pll_bandwidth));
    }
    if(m_pll_damping_factor_isSet){
        obj->insert("pllDampingFactor", QJsonValue(pll_damping_factor));
    }
    if(m_pll_loop_gain_isSet){
        obj->insert("pllLoopGain", QJsonValue(pll_loop_gain));
    }
    if(m_input_type_isSet){
        obj->insert("inputType", QJsonValue(input_type));
    }
    if(m_rgb_color_isSet){
        obj->insert("rgbColor", QJsonValue(rgb_color));
    }
    if(title != nullptr && *title != QString("")){
        toJsonValue(QString("title"), title, obj, QString("QString"));
    }
    if(m_stream_index_isSet){
        obj->insert("streamIndex", QJsonValue(stream_index));
    }
    if(m_use_reverse_api_isSet){
        obj->insert("useReverseAPI", QJsonValue(use_reverse_api));
    }
    if(reverse_api_address != nullptr && *reverse_api_address != QString("")){
        toJsonValue(QString("reverseAPIAddress"), reverse_api_address, obj, QString("QString"));
    }
    if(m_reverse_api_port_isSet){
        obj->insert("reverseAPIPort", QJsonValue(reverse_api_port));
    }
    if(m_reverse_api_device_index_isSet){
        obj->insert("reverseAPIDeviceIndex", QJsonValue(reverse_api_device_index));
    }
    if(m_reverse_api_channel_index_isSet){
        obj->insert("reverseAPIChannelIndex", QJsonValue(reverse_api_channel_index));
    }
    if((spectrum_config != nullptr) && (spectrum_config->isSet())){
        toJsonValue(QString("spectrumConfig"), spectrum_config, obj, QString("SWGGLSpectrum"));
    }
    if((scope_config != nullptr) && (scope_config->isSet())){
        toJsonValue(QString("scopeConfig"), scope_config, obj, QString("SWGGLScope"));
    }
    if((channel_marker != nullptr) && (channel_marker->isSet())){
        toJsonValue(QString("channelMarker"), channel_marker, obj, QString("SWGChannelMarker"));
    }

    return obj;
}

qint32
SWGChannelAnalyzerSettings::getFrequency() {
    return frequency;
}
void
SWGChannelAnalyzerSettings::setFrequency(qint32 frequency) {
    this->frequency = frequency;
    this->m_frequency_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getDownSample() {
    return down_sample;
}
void
SWGChannelAnalyzerSettings::setDownSample(qint32 down_sample) {
    this->down_sample = down_sample;
    this->m_down_sample_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getDownSampleRate() {
    return down_sample_rate;
}
void
SWGChannelAnalyzerSettings::setDownSampleRate(qint32 down_sample_rate) {
    this->down_sample_rate = down_sample_rate;
    this->m_down_sample_rate_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getBandwidth() {
    return bandwidth;
}
void
SWGChannelAnalyzerSettings::setBandwidth(qint32 bandwidth) {
    this->bandwidth = bandwidth;
    this->m_bandwidth_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getLowCutoff() {
    return low_cutoff;
}
void
SWGChannelAnalyzerSettings::setLowCutoff(qint32 low_cutoff) {
    this->low_cutoff = low_cutoff;
    this->m_low_cutoff_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getSpanLog2() {
    return span_log2;
}
void
SWGChannelAnalyzerSettings::setSpanLog2(qint32 span_log2) {
    this->span_log2 = span_log2;
    this->m_span_log2_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getSsb() {
    return ssb;
}
void
SWGChannelAnalyzerSettings::setSsb(qint32 ssb) {
    this->ssb = ssb;
    this->m_ssb_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getPll() {
    return pll;
}
void
SWGChannelAnalyzerSettings::setPll(qint32 pll) {
    this->pll = pll;
    this->m_pll_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getFll() {
    return fll;
}
void
SWGChannelAnalyzerSettings::setFll(qint32 fll) {
    this->fll = fll;
    this->m_fll_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getCostasLoop() {
    return costas_loop;
}
void
SWGChannelAnalyzerSettings::setCostasLoop(qint32 costas_loop) {
    this->costas_loop = costas_loop;
    this->m_costas_loop_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getRrc() {
    return rrc;
}
void
SWGChannelAnalyzerSettings::setRrc(qint32 rrc) {
    this->rrc = rrc;
    this->m_rrc_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getRrcRolloff() {
    return rrc_rolloff;
}
void
SWGChannelAnalyzerSettings::setRrcRolloff(qint32 rrc_rolloff) {
    this->rrc_rolloff = rrc_rolloff;
    this->m_rrc_rolloff_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getPllPskOrder() {
    return pll_psk_order;
}
void
SWGChannelAnalyzerSettings::setPllPskOrder(qint32 pll_psk_order) {
    this->pll_psk_order = pll_psk_order;
    this->m_pll_psk_order_isSet = true;
}

float
SWGChannelAnalyzerSettings::getPllBandwidth() {
    return pll_bandwidth;
}
void
SWGChannelAnalyzerSettings::setPllBandwidth(float pll_bandwidth) {
    this->pll_bandwidth = pll_bandwidth;
    this->m_pll_bandwidth_isSet = true;
}

float
SWGChannelAnalyzerSettings::getPllDampingFactor() {
    return pll_damping_factor;
}
void
SWGChannelAnalyzerSettings::setPllDampingFactor(float pll_damping_factor) {
    this->pll_damping_factor = pll_damping_factor;
    this->m_pll_damping_factor_isSet = true;
}

float
SWGChannelAnalyzerSettings::getPllLoopGain() {
    return pll_loop_gain;
}
void
SWGChannelAnalyzerSettings::setPllLoopGain(float pll_loop_gain) {
    this->pll_loop_gain = pll_loop_gain;
    this->m_pll_loop_gain_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getInputType() {
    return input_type;
}
void
SWGChannelAnalyzerSettings::setInputType(qint32 input_type) {
    this->input_type = input_type;
    this->m_input_type_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getRgbColor() {
    return rgb_color;
}
void
SWGChannelAnalyzerSettings::setRgbColor(qint32 rgb_color) {
    this->rgb_color = rgb_color;
    this->m_rgb_color_isSet = true;
}

QString*
SWGChannelAnalyzerSettings::getTitle() {
    return title;
}
void
SWGChannelAnalyzerSettings::setTitle(QString* title) {
    this->title = title;
    this->m_title_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getStreamIndex() {
    return stream_index;
}
void
SWGChannelAnalyzerSettings::setStreamIndex(qint32 stream_index) {
    this->stream_index = stream_index;
    this->m_stream_index_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getUseReverseApi() {
    return use_reverse_api;
}
void
SWGChannelAnalyzerSettings::setUseReverseApi(qint32 use_reverse_api) {
    this->use_reverse_api = use_reverse_api;
    this->m_use_reverse_api_isSet = true;
}

QString*
SWGChannelAnalyzerSettings::getReverseApiAddress() {
    return reverse_api_address;
}
void
SWGChannelAnalyzerSettings::setReverseApiAddress(QString* reverse_api_address) {
    this->reverse_api_address = reverse_api_address;
    this->m_reverse_api_address_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getReverseApiPort() {
    return reverse_api_port;
}
void
SWGChannelAnalyzerSettings::setReverseApiPort(qint32 reverse_api_port) {
    this->reverse_api_port = reverse_api_port;
    this->m_reverse_api_port_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getReverseApiDeviceIndex() {
    return reverse_api_device_index;
}
void
SWGChannelAnalyzerSettings::setReverseApiDeviceIndex(qint32 reverse_api_device_index) {
    this->reverse_api_device_index = reverse_api_device_index;
    this->m_reverse_api_device_index_isSet = true;
}

qint32
SWGChannelAnalyzerSettings::getReverseApiChannelIndex() {
    return reverse_api_channel_index;
}
void
SWGChannelAnalyzerSettings::setReverseApiChannelIndex(qint32 reverse_api_channel_index) {
    this->reverse_api_channel_index = reverse_api_channel_index;
    this->m_reverse_api_channel_index_isSet = true;
}

SWGGLSpectrum*
SWGChannelAnalyzerSettings::getSpectrumConfig() {
    return spectrum_config;
}
void
SWGChannelAnalyzerSettings::setSpectrumConfig(SWGGLSpectrum* spectrum_config) {
    this->spectrum_config = spectrum_config;
    this->m_spectrum_config_isSet = true;
}

SWGGLScope*
SWGChannelAnalyzerSettings::getScopeConfig() {
    return scope_config;
}
void
SWGChannelAnalyzerSettings::setScopeConfig(SWGGLScope* scope_config) {
    this->scope_config = scope_config;
    this->m_scope_config_isSet = true;
}

SWGChannelMarker*
SWGChannelAnalyzerSettings::getChannelMarker() {
    return channel_marker;
}
void
SWGChannelAnalyzerSettings::setChannelMarker(SWGChannelMarker* channel_marker) {
    this->channel_marker = channel_marker;
    this->m_channel_marker_isSet = true;
}


bool
SWGChannelAnalyzerSettings::isSet(){
    bool isObjectUpdated = false;
    do{
        if(m_frequency_isSet){
            isObjectUpdated = true; break;
        }
        if(m_down_sample_isSet){
            isObjectUpdated = true; break;
        }
        if(m_down_sample_rate_isSet){
            isObjectUpdated = true; break;
        }
        if(m_bandwidth_isSet){
            isObjectUpdated = true; break;
        }
        if(m_low_cutoff_isSet){
            isObjectUpdated = true; break;
        }
        if(m_span_log2_isSet){
            isObjectUpdated = true; break;
        }
        if(m_ssb_isSet){
            isObjectUpdated = true; break;
        }
        if(m_pll_isSet){
            isObjectUpdated = true; break;
        }
        if(m_fll_isSet){
            isObjectUpdated = true; break;
        }
        if(m_costas_loop_isSet){
            isObjectUpdated = true; break;
        }
        if(m_rrc_isSet){
            isObjectUpdated = true; break;
        }
        if(m_rrc_rolloff_isSet){
            isObjectUpdated = true; break;
        }
        if(m_pll_psk_order_isSet){
            isObjectUpdated = true; break;
        }
        if(m_pll_bandwidth_isSet){
            isObjectUpdated = true; break;
        }
        if(m_pll_damping_factor_isSet){
            isObjectUpdated = true; break;
        }
        if(m_pll_loop_gain_isSet){
            isObjectUpdated = true; break;
        }
        if(m_input_type_isSet){
            isObjectUpdated = true; break;
        }
        if(m_rgb_color_isSet){
            isObjectUpdated = true; break;
        }
        if(title && *title != QString("")){
            isObjectUpdated = true; break;
        }
        if(m_stream_index_isSet){
            isObjectUpdated = true; break;
        }
        if(m_use_reverse_api_isSet){
            isObjectUpdated = true; break;
        }
        if(reverse_api_address && *reverse_api_address != QString("")){
            isObjectUpdated = true; break;
        }
        if(m_reverse_api_port_isSet){
            isObjectUpdated = true; break;
        }
        if(m_reverse_api_device_index_isSet){
            isObjectUpdated = true; break;
        }
        if(m_reverse_api_channel_index_isSet){
            isObjectUpdated = true; break;
        }
        if(spectrum_config && spectrum_config->isSet()){
            isObjectUpdated = true; break;
        }
        if(scope_config && scope_config->isSet()){
            isObjectUpdated = true; break;
        }
        if(channel_marker && channel_marker->isSet()){
            isObjectUpdated = true; break;
        }
    }while(false);
    return isObjectUpdated;
}
}

