//
//  QttAudio.h
//
//  Copyright © 2016年 www.qttaudio.com. All rights reserved.
//

#ifndef QttAudioEngine_h
#define QttAudioEngine_h

#include <stdio.h>
#ifndef _BOOL_T_
#define _BOOL_T_
typedef unsigned char bool_t;
#endif /* _BOOL_T_ */
#undef TRUE
#undef FALSE
#define TRUE 1
#define FALSE 0


typedef void QttAudioEngine;
typedef void QttAudioStream;
typedef void QttAudioMixer;

typedef void (*QttCaptureCallback)(char *buf, size_t len, void *userdata);
typedef void (*OnEventCallback)(void *userdata, unsigned int event, void *arg);

typedef struct{
	char name[1024];
	char id[1024];
	bool_t supportPlayback;
	bool_t supportCapture;
}SoundDevice;

typedef struct{
	int count;
	SoundDevice *sd[];
}SoundDeviceList;

#ifdef _WIN32
#ifdef DLL_EXPORTS
#define QTT_PUBLIC __declspec(dllexport)
#else
#define QTT_PUBLIC __declspec(dllimport)
#endif
#else
#define QTT_PUBLIC
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    
    /**
     初始化QttAudioEngine，程序启动时调用

     @param appKey 网站注册时申请的appKey
     
     @return 如果为NULL表示初始化失败
     */
    QTT_PUBLIC QttAudioEngine *qtt_engine_init(const char *appKey);
    
    /**
     释放QttAudioEngine，程序退出时调用
     
     @param engine
     */
    QTT_PUBLIC void qtt_engine_free(QttAudioEngine *engine);
    
    
    /**
     设置mic采集回调函数，通过设置该回调，开发者可以源源不断获得mic采集的音频数据。
     该接口与qtt_stream_set_capture_callback不同之处在于，该接口只获取mic采集的声音，而stream回调包含了混音部分
     
     @param engine QttAudioEngine指针
     @param callback 回调函数
     @param userdata
     */
    QTT_PUBLIC void qtt_engine_set_mic_capture_callback(QttAudioEngine *engine, QttCaptureCallback callback, void *userdata);
    
    /**
     设置mic采集数据的编码格式
     
     @param engine QttAudioEngine指针
     @param mime 可以是"opus","speex","pcm"
     @param channel 1单声道或者2双声道
     @param samplerate 采样率，一般是8000，16000，32000，44100，48000
     @param bitrate 比特率，一般是16000，28000，32000，48000，64000，96000等；当mime设置为“pcm"时，比特率设置由channel和samplerate决定了，该参数无效，设置为0即可
     */
    QTT_PUBLIC void qtt_engine_set_mic_codec_params(QttAudioEngine *engine, char *mime, int channel, int samplerate, int bitrate);
    
    
    /**
     如果qtt_engine_set_mic_codec_params设置为pcm编码，可以使用该接口设置采集回调的buffer size
     
     @param engine QttAudioEngine指针
     @param bufsize 该engine的QttCaptureCallback的buf大小，只对pcm编码有效
     */
    QTT_PUBLIC void qtt_engine_set_pcm_buf_size(QttAudioEngine *engine, int bufsize);
    
    /**
     开启\关闭 噪音抑制
     
     @param engine QttAudioEngine指针
     @param val true: 开启; false: 关闭
     */
    QTT_PUBLIC void qtt_engine_enable_noise_suppression(QttAudioEngine *engine, bool_t val);
    
    
    /**
     设置噪音抑制的阈值
     
     @param engine QttAudioEngine指针
     @param val 默认为0.05f，开发者可根据实际场景对该值进行调节
     */
    QTT_PUBLIC void qtt_engine_set_ng_thres(QttAudioEngine *engine, float val);
    
    /**
     检测回声时延
     对与某些特殊机型有回声的情况，调用该函数测量该机型的回声时延
     
     @param engine QttAudioEngine指针
     @return 回声时延，毫秒
     */
    QTT_PUBLIC int qtt_engine_detect_echo(QttAudioEngine *engine);
    
    
    /**
     是否需要检测回声时延
     
     @param engine QttAudioEngine指针
     @param needDetect true为需要，false为不需要
     */
    QTT_PUBLIC void qtt_engine_need_detect_echo(QttAudioEngine *engine, bool_t needDetect);
    
    
    /**
     设置回声时延
     
     @param engine QttAudioEngine指针
     @param delay 毫秒
     */
    QTT_PUBLIC void qtt_engine_set_delay(QttAudioEngine *engine, int delay);
    
    
    
    
    /**
     设置mic的音量
     
     @param engine QttAudioEngine指针
     @param gain 增益百分比，1f表示100%，0表示无声，可以大于1f，建议在0-3f之间
     */
    QTT_PUBLIC void qtt_engine_set_mic_volume(QttAudioEngine *engine, float gain);
    
    
    /**
     设置静音事件检测回调函数
     
     @param engine QttAudioEngine指针
     @param eventNotify 事件回调函数
     @param userdata 用户传的任意data
     */
    QTT_PUBLIC void qtt_engine_set_silence_detected(QttAudioEngine *engine, OnEventCallback eventNotify, void *userdata);
    
    
    /**
     设置静音事件检测的阈值
     
     @param engine QttAudioEngine指针
     @param val 阈值默认为0.01f，开发者可根据实际场景调节它
     */
    QTT_PUBLIC void qtt_engine_set_silence_detected_threshold(QttAudioEngine *engine, float val);
    
#if defined(_WIN32) || defined(__linux__) && !defined(__ANDROID__)
    /**
     获得采集和播放的声音设备列表
     
     @param engine QttAudioEngine指针
     @param cardList 声音设备列表数组

	 @return 返回声音设备数量
     */
	QTT_PUBLIC int qtt_get_card_list(QttAudioEngine *engine, SoundDevice *cardArray);
    
    
    /**
     设置声音播放设备
     
     @param engine QttAudioEngine指针
     @param id SoundDevice的id
     */
    QTT_PUBLIC void qtt_set_play_card_by_id(QttAudioEngine *engine, char *id);
    
    
    /**
     设置声音采集设备
     
     @param engine QttAudioEngine指针
     @param id SoundDevice的id
     */
    QTT_PUBLIC void qtt_set_capt_card_by_id(QttAudioEngine *engine, char *id);
    
    
    /**
     设置一个mixer可混音的stream最大数量,默认是20,大于2000无效

     @param engine QttAudioEngine指针
     @param num 数量
     */
    QTT_PUBLIC void qtt_engine_set_max_stream_of_mixer(QttAudioEngine *engine, int num);

    /**
     创建一个QttAudioMixer混音器
     
     @param engine QttAudioEngine指针
     @return 如果为NULL表示创建QttAudioMixer失败
     */
    QTT_PUBLIC QttAudioMixer *qtt_engine_create_mixer(QttAudioEngine *engine);
    
    /**
     创建一个属于QttAudioMixer混音器的QttAudioStream，用QttAudioMixer创建的QttAudioStream不会涉及声音的采集与播放，只是纯粹用来作为混音使用
     
     @param mixer QttAudioMixer指针
     @return 如果为NULL表示创建QTTAudioStream失败
     */
    QTT_PUBLIC QttAudioStream *qtt_mixer_create_stream(QttAudioMixer *mixer);
    
    /**
     当不使用该mixer时，请及时释放它
     在调用该函数前，请保证该mixer创建的所有QTTAudioStream对象都stop或free了
     
     @param mixer QttAudioMixer指针
     @return 0，成功；
     */
    QTT_PUBLIC int qtt_mixer_free(QttAudioMixer *mixer);
#endif
    /**
     创建QttAudioStream
     
     @param engine QttAudioEngine指针
     @return 如果为NULL表示创建QTTAudioStream失败
     */
    QTT_PUBLIC QttAudioStream *qtt_engine_create_stream(QttAudioEngine *engine);
    
    
    /**
     通过id获得QttAudioStream
     
     @param engine QttAudioEngine指针
     @param id QttAudioStream的id
     @return QttAudioStream
     */
    QTT_PUBLIC QttAudioStream *qtt_engine_get_stream_by_id(QttAudioEngine *engine, long id);
    
    /**
     android用户使用，某些android特殊机型如声音无法正常采集或播放，需要调用该函数，并将val置为true
     
     @param engine QttAudioEngine指针
     @param val true: 打开兼容模式; false: 关闭兼容模式
     */
    QTT_PUBLIC void qtt_engine_set_compatibility_audio_mode(QttAudioEngine *engine, bool_t val);
	
	
	QTT_PUBLIC int qtt_engine_start(QttAudioEngine *engine);
	
	
	QTT_PUBLIC void qtt_engine_stop(QttAudioEngine *engine);
    
    /**
     设置音频流解码参数,默认为pcm,1,16000
     
     @param stream     QttAudioStream指针
     @param mime       "opus","speex","pcm"
     @param channel    1单声道或者2双声道
     @param samplerate 采样率，一般是8000，16000，32000，44100，48000
     @param bitrate    比特率，一般是16000，28000，32000，48000，64000，96000等；当mime设置为“pcm"时，比特率设置由channel和samplerate决定了，该参数无效，设置为0即可
     */
    QTT_PUBLIC void qtt_stream_set_codec_params(QttAudioStream *stream, char *mime, int channel, int samplerate, int bitrate);
    
    
    /**
     对qtt_stream_set_codec_params的包装，内置默认参数
     
     @param stream QttAudioStream指针
     @param mime "opus","speex","pcm"
     */
    QTT_PUBLIC void qtt_stream_set_codec_default_params(QttAudioStream *stream, char *mime);
    
    
//    /**
//     如果qtt_stream_set_codec_params设置为pcm编码，可以使用该参数设置采集回调的buffer size
//
//     @param stream QttAudioStream指针
//     @param bufsize 该stream的QttCaptureCallback的buf大小，只对pcm编码有效
//     */
//    QTT_PUBLIC void qtt_stream_set_pcm_buf_size(QttAudioStream *stream, int bufsize);

    
    
    /**
     针对windows
     
     @param stream QttAudioStream指针
     @param out 输出字符串
     */
    QTT_PUBLIC void qtt_stream_get_fmtp(QttAudioStream *stream, char *out);
    
    
    /**
     获得QttAudioStream的唯一标识id
     
     @param stream QttAudioStream指针
     @return id
     */
    QTT_PUBLIC long qtt_stream_get_id(QttAudioStream *stream);
    
    /**
     设置RTP网络收发参数
     QttAudioStream内置RTP收发模块，如果开发者采用RTP协议进行音频数据传输，则直接调用该接口设置收发参数
     
     @param stream         QttAudioStream指针
     @param localRtpPort   本地rtp端口
     @param localRtcpPort  本地rtcp端口
     @param remoteIP       远程目的ip
     @param remoteRtpPort  远程目的rtp端口
     @param remoteRtcpPort 远程目的rtcp端口
     @param mode           1是只接收模式，2是发送&接收模式
     @param ssrc           设置rtp的ssrc，该字段一般用来区分rtp流
     @param idx            设置rtp的载荷类型，通话双方必须要一样
     */
    QTT_PUBLIC void qtt_stream_set_rtp_params(QttAudioStream *stream, int localRtpPort, int localRtcpPort, char *remoteIP, int remoteRtpPort, int remoteRtcpPort, int mode, int ssrc, int idx);
    
//    /**
//     设置stream为文件流，并且设置pcm输入参数;当stream为文件流时，影响的是qtt_stream_write_payload接口，只接受PCM格式写入；
//     文件流与实时流不同的地方在于，文件流会保证write的buffer完整，适合播放音乐；但不适合实时通话
//
//     @param stream QttAudioStream指针
//     @param pcm_channel pcm声道数
//     @param pcm_samplerate pcm采样率
//     */
//    QTT_PUBLIC void qtt_stream_set_as_file_input(QttAudioStream *stream, int pcm_channel, int pcm_samplerate);

    
    /**
     启动QTTAudioStream
     
     @param stream QttAudioStream指针
     
     @return 0为成功，其他为失败
     */
    QTT_PUBLIC int qtt_stream_start(QttAudioStream *stream);
    
    /**
     是否静音QttAudioStream
     
     @param stream QttAudioStream指针
     @param mute   true为静音，false为不静音
     */
    QTT_PUBLIC void qtt_stream_mute(QttAudioStream *stream, bool_t mute);
    
    /**
     设置播放的音量
     
     @param stream QttAudioStream指针
     @param val    增益百分比，1f表示100%，0表示无声，可以大于1f，建议在0-3f之间
     */
    QTT_PUBLIC void qtt_stream_set_output_volume(QttAudioStream *stream, float val);
    
//    /**
//     设置输入的音量
//
//     @param stream QttAudioStream指针
//     @param val    增益百分比，1f表示100%，0表示无声，可以大于1f，建议在0-3f之间
//     */
//    QTT_PUBLIC void qtt_stream_set_input_volume(QttAudioStream *stream, float val);
    
    
    /**
     停止QttAudioStream
     
     @param stream QttAudioStream指针
     */
    QTT_PUBLIC void qtt_stream_stop(QttAudioStream *stream);
    
    /**
     释放QttAudioStream
     
     @param stream QttAudioStream指针
     */
    QTT_PUBLIC void qtt_stream_free(QttAudioStream *stream);
    
    /**
	 deprecated
     设置QttAudioStream的输出回调函数，通过设置该回调，开发者可以源源不断获得该stream输出的音频数据
     
     @param stream   QttAudioStream指针
     @param callback 音频回调接口
     @param userdata 用户数据
     */
    QTT_PUBLIC void qtt_stream_set_capture_callback(QttAudioStream *stream, QttCaptureCallback callback, void *userdata);
    
    /**
     向QttAudioStream里写入音频buf
     如果该QttAudioStream是QttAudioEngine创建的，则会播放出声音来
     
     @param stream
     @param buf    音频buf
     @param len    音频buf的长度，大小不建议大于1400
     */
    QTT_PUBLIC void qtt_stream_write_payload(QttAudioStream *stream, char *buf, size_t len);
    
    
#ifdef __cplusplus
}
#endif

#endif /* QTTAudioEngine_h */
