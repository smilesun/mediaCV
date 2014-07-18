#ifndef CEPSILONFILTER_H
#define CEPSILONFILTER_H
#include "cimgproc.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class CepsilonFilter : public CImgProc
{
public:
    CepsilonFilter();
    void process(const cv::Mat &image, cv::Mat &result);//virtual
    const cv::Mat* pimage;
    cv::Mat* presult;
private:
    void epsilonFilter();
    uchar getNeighborWeight(int row,int col,int k);
};

#endif // CEPSILONFILTER_H
