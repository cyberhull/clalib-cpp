/*
 * C++ Port of the SLALIB library.
 * Written by Vadim Sytnikov.
 * Copyright (C) 2021 CyberHULL, Ltd.
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef SLALIB_H_INCLUDED
#define SLALIB_H_INCLUDED

#include <type_traits>

namespace sla {

/// Catalogues of fundamental stars: celestial reference frames.
enum Catalogue {
    CAT_NONE, ///< Unspecified
    CAT_FC,   ///< Fundamental-Catalog by Auwers [1879], 539 stars
    CAT_NFK,  ///< Neuer Fundamentalkatalog by J. Peters, 925 stars
    CAT_FK3,  ///< Third Fundamental Catalogue by Kopff [1937]
    CAT_FK4,  ///< Fourth Fundamental Catalogue [1963], 1,535 stars
    CAT_FK5,  ///< Fifth Fundamental Catalogue [1988], 1,535 stars
    CAT_FK6   ///< Sixth Fundamental Catalogue [2000], 4,150 stars
};

/// Status codes for the caf2r() procedure (degrees to radians conversion).
enum D2RStatus {
    D2R_OK = 0,         ///< all arguments fit their ranges, conversion successful
    D2R_BAD_DEGREES,    ///< degrees outside of range [0..359]
    D2R_BAD_ARCMINUTES, ///< minutes outside of range [0..59]
    D2R_BAD_ARCSECONDS  ///< seconds outside of range [0..60)
};

/// Status codes for the cldj(), caldj(), clyd(), and calyd() procedures (Gregorian to Julian calendar conversions).
enum G2JStatus {
    G2J_OK = 0,    ///< all arguments fit their ranges, conversion successful
    G2J_BAD_YEAR,  ///< year earlier (less) than -4699, output value(s) not calculated/returned
    G2J_BAD_MONTH, ///< month outside of range [1..12], output value(s) not calculated/returned
    G2J_BAD_DAY    ///< day outside [0..<days-in-given-month>] range, BUT output value(s) were calculated and returned
};

/// Status codes for the dtf2d() and ctf2d() procedures.
enum T2DStatus {
    T2D_OK,          ///< all arguments fit their ranges, conversion successful
    T2D_BAD_HOURS,   ///< hours outside of range [0..23]
    T2D_BAD_MINUTES, ///< minutes outside of range [0..59]
    T2D_BAD_SECONDS  ///< seconds outside of range [0..60)
};

/// Generic 3-component vector of floating-point elements.
template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
using Vector = T[3];

/// Generic 3x3 matrix of floating-point elements.
template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
using Matrix = T[3][3];

/**
 * Representation of partial spherical coordinates (direction-only): longitude/latitude, or right ascension/
 * declination, or hour angle/declination.
 */
template <typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
struct Spherical {
    T s_a; ///< longitude or RA (radians)
    T s_b; ///< latitude or Dec (radians)

    // accessors for the first field
    [[nodiscard]] T get_longitude() const { return s_a; }
    void set_longitude(T radians) { s_a = radians; }
    [[nodiscard]] T get_ra() const { return s_a; }
    void set_ra(T radians) { s_a = radians; }
    [[nodiscard]] T get_ha() const { return s_a; }
    void set_ha(T radians) { s_a = radians; }

    // accessors for the second field
    [[nodiscard]] T get_latitude() const { return s_b; }
    void set_latitude(T radians) { s_b = radians; }
    [[nodiscard]] T get_dec() const { return s_b; }
    void set_dec(T radians) { s_b = radians; }
};

/// Representation of full spherical coordinates: longitude, latitude, and distance.
template <typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
struct SphericalPos: public Spherical<T> {
    T sp_dist; ///< distance along long/lat ray

    [[nodiscard]] T get_dist() const { return sp_dist; }
    void set_dist(T dist) { sp_dist = dist; }
};

/// Representation of a date of Gregorian calendar.
struct Date {
    int    d_year;      ///< Year
    int    d_month;     ///< Month, [1..12]
    int    d_day;       ///< Day, [1..365]
    int    d_ifraction; ///< Fraction of the day, digits to the right of the decimal point
    double d_fraction;  ///< Fraction of the day, [0..1.0f)
};

/**
 * Representation os various conversion results: days to hours, minutes, seconds; or radians to degrees, arcminutes,
 * arcseconds; etc. The same data structure has to be passed between routines interpreting it quite differently,
 * hence this implementation, having different interfaces to the same underlying data structure.
 */
class ConversionResult {
    int  cr_data[4]; ///< hours/minutes/seconds/fraction or degrees/arcminutes/arcseconds/fraction
    bool cr_sign;    ///< `false` for '-', `true` for '+'

public:
    // interface for radians/days to hours, minutes, seconds conversions
    [[nodiscard]] int get_hours() const { return cr_data[0]; }
    void set_hours(int hours) { cr_data[0] = hours; }
    [[nodiscard]] int get_minutes() const { return cr_data[1]; }
    void set_minutes(int minutes) { cr_data[1] = minutes; }
    [[nodiscard]] int get_seconds() const { return cr_data[2]; }
    void set_seconds(int seconds) { cr_data[2] = seconds; }

    // interface for radians to degrees, arcminutes, arcseconds conversion
    [[nodiscard]] int get_degrees() const { return cr_data[0]; }
    void set_degrees(int degrees) { cr_data[0] = degrees; }
    [[nodiscard]] int get_arcminutes() const { return cr_data[1]; }
    void set_arcminutes(int arcminutes) { cr_data[1] = arcminutes; }
    [[nodiscard]] int get_arcseconds() const { return cr_data[2]; }
    void set_arcseconds(int arcseconds) { cr_data[2] = arcseconds; }

    // interface shared by all conversions
    [[nodiscard]] int get_fraction() const { return cr_data[3]; }
    void set_fraction(int fraction) { cr_data[3] = fraction; }
    [[nodiscard]] char get_sign() const { return cr_sign? '+': '-'; }
    void set_sign(char sign) { cr_sign = sign == '+'; }
    void set_sign(bool sign) { cr_sign = sign; }
};

// auxiliary functions (used internally by API functions)
int process_year_defaults(int year);
G2JStatus validate_gregorian_day(int year, int month, int day);

// library API (documentation can be found in the implementation files)
double airmas(double zenith_dist);
float range(float angle);
double drange(double angle);
float ranorm(float angle);
double dranrm(double angle);
void av2m(const Vector<float> vec, Matrix<float> mat);
void dav2m(const Vector<double> vec, Matrix<double> mat);
void cc2s(const Vector<float> cartesian, Spherical<float>& spherical);
void dcc2s(const Vector<double> cartesian, Spherical<double>& spherical);
void cs2c(const Spherical<float>& spherical, Vector<float> cartesian);
void dcs2c(const Spherical<double>& spherical, Vector<double> cartesian);
void euler(const char* order, float phi, float theta, float psi, Matrix<float> mat);
void deuler(const char* order, double phi, double theta, double psi, Matrix<double> mat);
void imxv(const Matrix<float> mat, const Vector<float> va, Vector<float> vb);
void dimxv(const Matrix<double> mat, const Vector<double> va, Vector<double> vb);
void m2av(const Matrix<float> mat, Vector<float> axis);
void dm2av(const Matrix<double> mat, Vector<double> axis);
void mxm(const Matrix<float> ma, const Matrix<float> mb, Matrix<float> mc);
void dmxm(const Matrix<double> ma, const Matrix<double> mb, Matrix<double> mc);
void mxv(const Matrix<float> mat, const Vector<float> va, Vector<float> vb);
void dmxv(const Matrix<double> mat, const Vector<double> va, Vector<double> vb);
float vdv(const Vector<float> va, const Vector<float> vb);
double dvdv(const Vector<double> va, const Vector<double> vb);
float vn(const Vector<float> vec, Vector<float> nvec);
double dvn(const Vector<double> vec, Vector<double> nvec);
void vxv(const Vector<float> va, const Vector<float> vb, Vector<float> vc);
void dvxv(const Vector<double> va, const Vector<double> vb, Vector<double> vc);
double zd(const Spherical<double>& dir, double phi);
double pa(double ha, double dec, double phi);
double bear(float a1, float b1, float a2, float b2);
double dbear(double a1, double b1, double a2, double b2);
float pav(const Vector<float> va, const Vector<float> vb);
double dpav(const Vector<double> va, const Vector<double> vb);
void e2h(float ha, float dec, float phi, float& azimuth, float& elevation);
void de2h(double ha, double dec, double phi, double& azimuth, double& elevation);
void h2e(float azimuth, float elevation, float phi, float& ha, float& dec);
void dh2e(double azimuth, double elevation, double phi, double& ha, double& dec);
D2RStatus caf2r(int degrees, int minutes, float seconds, float& radians);
D2RStatus daf2r(int degrees, int minutes, double seconds, double& radians);
G2JStatus cldj(int year, int month, int day, double& mjd);
G2JStatus caldj(int year, int month, int day, double& mjd);
G2JStatus clyd(int year, int month, int day, int& jyear, int& jday);
G2JStatus calyd(int year, int month, int day, int& jyear, int& jday);
bool djcal(int ndp, double mjd, Date& date);
bool djcl(double mjd, Date& date);
void cd2tf(int ndp, float days, ConversionResult& result);
void dd2tf(int ndp, double days, ConversionResult& result);
void cr2af(int ndp, float angle, ConversionResult& result);
void dr2af(int ndp, double angle, ConversionResult& result);
void cr2tf(int ndp, float angle, ConversionResult& result);
void dr2tf(int ndp, double angle, ConversionResult& result);
T2DStatus ctf2d(int hours, int minutes, float seconds, float& days);
T2DStatus dtf2d(int hours, int minutes, double seconds, double& radians);
T2DStatus ctf2r(int hours, int minutes, float seconds, float & radians);
T2DStatus dtf2r(int hours, int minutes, double seconds, double& radians);
double dat(double utc);
double dt(double epoch);
double dtt(double utc);
double rcc(double tdb, double ut1, double cl, double cda, double cdp);
void atmdsp(double atk, double apm, double arh, double wl1, double a1, double b1, double wl2, double& a2, double& b2);
void refcoq(double atk, double apm, double arh, double wl, double& refa, double& refb);
double refro(double ozd, double oh, double atk, double apm, double arh, double wl, double phi, double tlr, double eps);
void refco(double oh, double atk, double apm, double arh, double wl, double phi, double tlr, double eps,
    double& refa, double& refb);
void refv(const Vector<double> vec, double refa, double refb, Vector<double> rvec);
double refz(double zu, double refa, double refb);
void ecmat(double date, Matrix<double> mat);
bool dmat(int n, Matrix<double> mat, Vector<double> vec, double& det, int ws[3]);
bool smat(int n, float* mat, float* vec, float& det, int* ws);
void altaz(double ha, double dec, double phi, double& az, double& az_vel, double& az_acc,
    double& el, double& el_vel, double& el_acc, double& pa, double& pa_vel, double& pa_acc);
void nutc(double tdb, double& psi, double& eps, double& eps0);
void nut(double tdb, Matrix<double> mat);
void nutc80(double tdb, double& psi, double& eps, double& eps0);
double epj2d(double je);
double epj(double mjd);
double epb2d(double be);
double epb(double mjd);
void prec(double ep0, double ep1, Matrix<double> mat);
void precl(double ep0, double ep1, Matrix<double> mat);
void prenut(double je, double mjd, Matrix<double> mat);
double dsepv(const Vector<double> va, const Vector<double> vb);
float sepv(const Vector<float> va, const Vector<float> vb);
double dsep(const Spherical<double>& sa, const Spherical<double>& sb);
float sep(const Spherical<float>& sa, const Spherical<float>& sb);
void prebn(double be0, double be1, Matrix<double> mat);
void preces(Catalogue system, double ep0, double ep1, Spherical<double>& pos);
void supgal(const Spherical<double>& sgal, Spherical<double>& gal);
float rverot(float phi, const Spherical<float>& pos, float stime);
float rvgalc(const Spherical<float>& pos);
float rvlg(const Spherical<float>& pos);
float rvlsrd(const Spherical<float>& pos);
float rvlsrk(const Spherical<float>& pos);
void wait(float seconds);

} // sla namespace

#endif // SLALIB_H_INCLUDED
