//
// Created by bush on 29/01/18.
//

#ifndef MPEVAL_ZP_H
#define MPEVAL_ZP_H

#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>
#include <NTL/vec_ZZ_p.h>
#include <chrono>
#include <iostream>
#include <sys/resource.h>

using namespace std;
using namespace NTL;
using namespace chrono;

void test_interpolation_result_zp(ZZ_pX &P, ZZ_p *x, ZZ_p *y, long degree);
void print_poly(ZZ_pX &P);
void interpolate_zp(ZZ_pX &resultP, ZZ_p *x, ZZ_p *y, long degree);
void test_multipoint_eval_zp(ZZ prime, long degree);
void test_interpolate_zp(ZZ prime, long degree);

#endif // MPEVAL_ZP_H
