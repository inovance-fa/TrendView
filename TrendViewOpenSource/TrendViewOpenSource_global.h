#ifndef TRENDVIEWOPENSOURCE_GLOBAL_H
#define TRENDVIEWOPENSOURCE_GLOBAL_H
#pragma once

#ifndef BUILD_STATIC
#if defined(TRENDVIEWOPENSOURCE_LIB)
#ifdef WIN32
#define TRENDVIEWOPENSOURCE_EXPORT __declspec(dllexport)
#else
#define TRENDVIEWOPENSOURCE_EXPORT 
#endif 
#else
#ifdef WIN32
#define TRENDVIEWOPENSOURCE_EXPORT __declspec(dllimport)
#else
#define TRENDVIEWOPENSOURCE_EXPORT 
#endif 
#endif
#else
#define TRENDVIEWOPENSOURCE_EXPORT
#endif

#endif


