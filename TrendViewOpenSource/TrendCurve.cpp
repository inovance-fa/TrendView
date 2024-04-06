#include "TrendCurve.h"
using namespace Graph;

TrendCurve::TrendCurve(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPCurve(keyAxis, valueAxis)
{
}

QRect TrendCurve::clipRect() const
{
    return (mKeyAxis && mValueAxis) 
        ? EnlargeRect(mKeyAxis.data()->axisRect()->rect() & mValueAxis.data()->axisRect()->rect(),1,1)
        : QRect();
}

QRect TrendCurve::EnlargeRect(const QRect& rect, int dx, int dy) const
{
    return QRect(rect.left(), rect.top(), rect.width() + dx, rect.height() + dy);
}