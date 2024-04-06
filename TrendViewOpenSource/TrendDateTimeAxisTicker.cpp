#include "TrendDateTimeAxisTicker.h"
using namespace Graph;

double TrendDateTimeAxisTicker::getTickStep(const QCPRange& range) 
{
    mTickCount = 4;
    if (range.size() < (double)mTickCount)
    {
        mTickCount = 1;
    }
 
    double result = range.size() / double(mTickCount + 1e-10);
    if (result < 1)
    {
        result = 1.0;
    }
    else if (result < 3600 * 24)
    {
        result = range.size() / double(mTickCount + 1e-10);
    }
    else
    {
        const double secondsPerDay = 3600 * 24;
        result = cleanMantissa(result / secondsPerDay) * secondsPerDay;
    }
    return result;
}

int TrendDateTimeAxisTicker::getSubTickCount(double tickStep)
{
    int result = QCPAxisTicker::getSubTickCount(tickStep);
    switch (qRound(tickStep))
    {
        case 5 * 60:
        {
            result = 4;
            break;
        }
        case 10 * 60:
        {
            result = 1;
            break;
        }
        case 15 * 60:
        {
            result = 2;
            break;
        }
        case 30 * 60:
        {
            result = 1;
            break;
        }
        case 60 * 60:
        {
            result = 3;
            break;
        }
        case 3600 * 2:
        {
            result = 3;
            break;
        }
        case 3600 * 3:
        {
            result = 2;
            break;
        }
        case 3600 * 6:
        {
            result = 1;
            break;
        }
        case 3600 * 12:
        {
            result = 3;
            break;
        }
        case 3600 * 24:
        {
            result = 3;
            break;
        }
        default:
            break;
    }
    return result;
}

QString TrendDateTimeAxisTicker::getTickLabel(double tick, const QLocale& locale, QChar formatChar, int precision)
{
    Q_UNUSED(precision)
    Q_UNUSED(formatChar)
    Q_UNUSED(locale)

    bool negative = tick < 0;
    if (negative)
    {
        tick *= -1;
    }
    QString result = QDateTime::fromSecsSinceEpoch(tick).toString(mDateTimeFormat);
    if (negative)
    {
        result.prepend(QLatin1Char('-'));
    }
    return result;
}

QVector<double> TrendDateTimeAxisTicker::createTickVector(double tickStep, const QCPRange& range)
{
    auto step = range.size() / double(mTickCount + 1e-10);

    QVector<double> result;
    result.push_back(range.lower);
    if (1 == mTickCount)
    {
        result.push_back(range.upper);
    }
    else
    {
        for (int i = 0; i < mTickCount; ++i)
        {
            result.push_back(result.back() + step);
        }
    }

    if (4 == mTickCount && result.size() > 0)
    {
        result[result.size() - 1] = range.upper;
    }

    return result;
}