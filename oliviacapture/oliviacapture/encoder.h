//
//  encoder.h
//  oliviacapture
//
//  Created by xuhao on 9/15/14.
//  Copyright (c) 2014 xuhao. All rights reserved.
//
#ifndef  __OLIVIA__CAPTURE__H__
#define  __OLIVIA__CAPTURE__H__

#include <string>
#include <opencv2/opencv.hpp>
#include <functional>
#include <mutex>
extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
}

#define ONE_M 1048576

class encoder
{
private:
    int ki=0;
    AVCodecContext * c =nullptr;
    AVFrame * frame  = nullptr;
protected:
    std::function<int(char*,int)> fwrite;
    std::mutex pause_lock;
    bool running = true;
public:
    int height , width;
    AVCodec *codec;
    int init();
    virtual uchar ** get_data();
    AVFrame * mkframe();
    void run();
    void encodeframe();
    int write();
    int reset();
    int bit_rate;
    void stop();
    
};

class CV_encoder : public encoder
{
private:
    std::function<cv::Mat()> getFrame;
public:
    uchar ** get_data();
    CV_encoder(std::function<cv::Mat()>,std::function<int(char*,int)>,float rate_m );
    void start()
    {
        pause_lock.unlock();
    }
    void stop()
    {
        pause_lock.lock();
    }
};


#endif