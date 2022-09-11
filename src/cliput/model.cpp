#include "model.h"
#include "algor.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

Model::Model(QObject*parent)
    :QStandardItemModel(parent)
{
    this->setColumnCount(2);
    thumbPercent=0.2;
    countPicture=0;
    countVideo=0;
}

void Model::onrenew()
{
    for(int i=0;i<countVideo;i++)
    {
        this->item(i)->setIcon(QIcon(QPixmap::fromImage(thumbnail(this->item(i)->data().toString()))));
    }
}

void Model::addVideo(int arow,QStandardItem*item)
{
    this->insertRow(arow,item);
    QImage nail=thumbnail(item->data().toString());
    item->setIcon(QIcon(QPixmap::fromImage(nail)));
    QStandardItem* dur=new QStandardItem;
    dur->setData(duration);
    dur->setText(Mstime::toHMS(dur->data().toInt()));
    this->setItem(arow,1,dur);
}

//获取缩略图
QImage Model::thumbnail(QString url)
{
    AVFormatContext* formatContext=avformat_alloc_context();
    //路径转换为char*
    char file[200];
    std::string str=url.toStdString();
    std::strcpy(file,str.c_str());
    if(avformat_open_input(&formatContext,file,nullptr,nullptr)!=0)
    {
        return QImage();
    }

    if(avformat_find_stream_info(formatContext,nullptr)!=0)
    {
        avformat_close_input(&formatContext);
        return QImage();
    }

    AVPacket* pkt=av_packet_alloc();
    AVFrame* temp_frame=av_frame_alloc();
    SwsContext* sws_ctx=nullptr;
    int ret=0;
    QImage thumbnail;
    bool done=false;

    int videoStream=0;
    for(int i=0;i<int(formatContext->nb_streams)&&!done;i++)
    {
        if(formatContext->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoStream=i;
            const AVCodec* codec=avcodec_find_decoder(formatContext->streams[i]->codecpar->codec_id);
            AVCodecContext *codec_ctx=avcodec_alloc_context3(codec);
            //获取时长
            duration=formatContext->streams[i]->duration*1000/formatContext->streams[i]->time_base.den;

            avcodec_parameters_to_context(codec_ctx,formatContext->streams[i]->codecpar);

            avcodec_open2(codec_ctx,codec,nullptr);
            //必须使用视频流的duration
            int stp=thumbPercent*formatContext->streams[i]->duration;
            av_seek_frame(formatContext,videoStream,stp,0);
            while(av_read_frame(formatContext,pkt)>=0)
            {
                if(pkt->stream_index==videoStream)
                {
                    av_frame_unref(temp_frame);

                    while((ret = avcodec_receive_frame(codec_ctx, temp_frame)) == AVERROR(EAGAIN))
                    {
                        ret=avcodec_send_packet(codec_ctx,pkt);
                        if(ret<0)
                        {
                            //fail to send packet
                            break;
                        }
                    }
                    if(ret<0&&ret!=AVERROR_EOF)
                    {
                        //fail to receive packet
                        continue;
                    }

                    int dstH=160;
                    int dstW=qRound(dstH*(float(temp_frame->width))/float(temp_frame->height));

                    sws_ctx=sws_getContext(
                                temp_frame->width,
                                temp_frame->height,
                                static_cast<AVPixelFormat>(temp_frame->format),
                                dstW,
                                dstH,
                                static_cast<AVPixelFormat>(AV_PIX_FMT_RGBA),
                                SWS_FAST_BILINEAR,
                                nullptr,
                                nullptr,
                                nullptr
                                );
                    int linesize[AV_NUM_DATA_POINTERS];
                    linesize[0]=dstW*4;

                    thumbnail=QImage(dstW,dstH,QImage::Format_RGBA8888);
                    uint8_t* data=thumbnail.bits();
                    sws_scale(sws_ctx,
                              temp_frame->data,
                              temp_frame->linesize,
                              0,
                              temp_frame->height,
                              &data,
                              linesize);
                    sws_freeContext(sws_ctx);
                    avcodec_close(codec_ctx);
                    avcodec_free_context(&codec_ctx);
                    done=true;
                    break;
                }
            }
        }

        av_frame_free(&temp_frame);
        av_packet_free(&pkt);
        avformat_close_input(&formatContext);
        if(done)
        {
            return thumbnail;
        }
    }
    return QImage();
}

void Model::setThumbPersent(int p)
{
    thumbPercent=p/100.00;
}






