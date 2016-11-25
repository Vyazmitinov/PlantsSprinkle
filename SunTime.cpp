#include "SunTime.h"

// Based on http://williams.best.vwh.net/sunrise_sunset_example.htm

#include <math.h>

int getDayOfTheYear(int day, int month, int year) {
  int N1 = floor(275 * month / 9);
  int N2 = floor((month + 9) / 12);
  int N3 = (1 + floor((year - 4 * floor(year / 4) + 2) / 3));
  return N1 - (N2 * N3) + day - 30;
}

inline float longitudeToHour(float longitude) {
  return longitude / 15;
}

float aproximateTime(int dayOfYear, float longitudeHour, bool rising) {
  if (rising) {
    return dayOfYear + ((6 - longitudeHour) / 24);
  }
  return dayOfYear + ((18 - longitudeHour) / 24);
}

inline float sunMeanAnomaly(float aproxTime) {
  return (0.9856 * aproxTime) - 3.289;
}

inline float degreeToRad(float degree) {
  return degree * 0.0174533;
}

inline float radToDegree(float rad) {
  return rad * 57.2957795131;
}

inline float ajustTo360(float angle) {
  if (angle < 0) {
    return angle + 360;
  } else if (angle >= 360) {
    return angle - 360;
  }
  return angle;
}

float sunTrueLongitude(float sunMA) {
  float sunMARad = degreeToRad(sunMA);
  float L = sunMA + (1.916 * sin(sunMARad)) + (0.020 * sin(2 * sunMARad)) + 282.634;
  return ajustTo360(L);
}

inline float sunRightAscension(float sunTL) {
  return ajustTo360(radToDegree(atan(0.91764 * tan(degreeToRad(sunTL)))));
}

float sunRAToLQuadrant(float sunTL, float sunRA) {
  float Lquadrant  = (floor(sunTL/90)) * 90;
  float RAquadrant = (floor(sunRA/90)) * 90;
  return sunRA + (Lquadrant - RAquadrant);
}

inline float sunRAToHours(float sunRA) {
  return sunRA / 15;
}

inline float sunSinDeclination(float sunTL) {
  return 0.39782 * sin(degreeToRad(sunTL));
}

inline float sunCosDeclination(float sunSinDec) {
  return cos(asin(sunSinDec));
}


float sunLocalHourAngle(float sunSinDec, float sunCosDec, bool rising, float latitude) {
  const float cosZenith = -0.0174531;//-0.01454;
  float cosH = (cosZenith - (sunSinDec * sin(degreeToRad(latitude))))
      / (sunCosDec * cos(degreeToRad(latitude)));
  if (cosH > 1) {
    //     the sun never rises on this location (on the specified date)
    return 0;
  } else if (cosH < -1) {
    //    the sun never sets on this location (on the specified date)
    return 0;
  }

  float H;
  if (rising) {
    H = 360 - radToDegree(acos(cosH));
  } else {
    H = radToDegree(acos(cosH));
  }
  return H / 15;
}

inline float localMeanTime(float sunLHA, float sunRAH, float aproxTime) {
  return sunLHA + sunRAH - (0.06571 * aproxTime) - 6.622;
}

float ajustToUtc(float localMT, float longitudeHour) {
  float UT = localMT - longitudeHour;
  if (UT < 0) {
    return UT + 24;
  }
  if (UT >= 24) {
    return UT - 24;
  }
  return UT;
}

uint8_t getMinutes(float time) {
  return (time - floor(time)) * 60;
}

float getSunTime(uint8_t day, uint8_t month, int year, bool sunrise, int timezone, float longitude, float latitude) {
  int dayOfYear = getDayOfTheYear(day, month, year);

  float longitudeHour = longitudeToHour(longitude);
  float aproxTime = aproximateTime(dayOfYear, longitudeHour, sunrise);

  float sunMA = sunMeanAnomaly(aproxTime);

  float sunTL = sunTrueLongitude(sunMA);

  float sunRA = sunRightAscension(sunTL);

  float sunRALQ = sunRAToLQuadrant(sunTL, sunRA);

  float sunRAH = sunRAToHours(sunRALQ);

  float sunSinDec = sunSinDeclination(sunTL);
  float sunCosDec = sunCosDeclination(sunSinDec);

  float sunLHA = sunLocalHourAngle(sunSinDec, sunCosDec, sunrise, latitude);

  float localMT = localMeanTime(sunLHA, sunRAH, aproxTime);

  float utc = ajustToUtc(localMT, longitudeHour);

  return utc + float(timezone);
}

