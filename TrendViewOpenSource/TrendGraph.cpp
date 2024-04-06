#include "TrendGraph.h"
using namespace Graph;

TrendGraph::TrendGraph(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPGraph(keyAxis, valueAxis)
{
}

QRect TrendGraph::clipRect() const
{
    return (mKeyAxis && mValueAxis) 
        ? EnlargeRect(mKeyAxis.data()->axisRect()->rect() & mValueAxis.data()->axisRect()->rect(), 1, 1)
        : QRect();
}

QRect TrendGraph::EnlargeRect(const QRect& rect, int dx, int dy) const
{
     return QRect(rect.left(), rect.top(), rect.width() + dx, rect.height() + dy);
}