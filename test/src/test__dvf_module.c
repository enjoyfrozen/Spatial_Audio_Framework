/*
 * Copyright 2020-2021 Leo McCormack, Michael McCrea
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @file test__dvf_module.c
 * @brief Unit tests for the SAF distance variation function (DVF) module.
 * @author Michael McCrea
 * @date 16.03.2021
 * @license ISC
 */

#include "saf_test.h"

void test__dvf_calcHighShelfParams(void){
    
    /* setup */
    const float acceptedTolerance = 0.00001f;
    const float acceptedTolerance_fc = 0.1f;
    const int nTheta = 19;
    const int nRho = 5;
    const float rho[nRho] = {1.150000,1.250000,1.570000,2.381000,3.990000};
    const float g0_ref[nRho][nTheta] = { /* testRhoCoeffs_g_0 */
        {22.670282,17.717752,11.902597,7.906282,4.720884,2.217061,0.134088,-1.613730,-3.095960,-5.279052,-5.433653,-6.342905,-7.107677,-7.744796,-8.236084,-8.613662,-8.864276,-9.065870,-9.089385},
        {18.295933,15.510441,11.452312,7.951083,4.997235,2.609075,0.592613,-1.107964,-2.557504,-4.547256,-4.853912,-5.750024,-6.504702,-7.133244,-7.621092,-7.993574,-8.244015,-8.438287,-8.467470},
        {11.937032,11.093339,9.245757,7.118216,4.990070,3.083402,1.371444,-0.121838,-1.427296,-2.979742,-3.542803,-4.381065,-5.091220,-5.683427,-6.149122,-6.508598,-6.748356,-6.923465,-6.961620},
        {6.676990,6.451424,5.818377,4.924700,3.861979,2.760683,1.662668,0.629080,-0.327831,-1.328149,-1.970549,-2.649238,-3.234743,-3.727775,-4.122829,-4.433178,-4.640902,-4.783351,-4.823625},
        {3.628860,3.534311,3.298166,2.922799,2.438587,1.888286,1.296135,0.698518,0.112899,-0.473626,-0.960644,-1.428032,-1.841763,-2.196404,-2.487131,-2.717121,-2.873915,-2.978235,-3.010937}
    };
    
    const float gInf_ref[nRho][nTheta] = { /* testRhoCoeffs_g_inf */
        {-4.643651,-4.225287,-4.134752,-4.386332,-5.244711,-6.439307,-7.659091,-8.887172,-10.004796,-10.694171,-11.190476,-10.876569,-10.140292,-9.913242,-9.411469,-8.981807,-8.723677,-8.529900,-8.574359},
        {-4.128221,-3.834507,-3.575000,-3.637788,-4.278932,-5.310000,-6.609705,-7.815000,-8.925450,-9.646588,-10.000000,-9.784733,-9.301643,-8.862963,-8.370815,-7.953778,-7.693305,-7.500645,-7.518260},
        {-3.094135,-2.963709,-2.721834,-2.573043,-2.793627,-3.414652,-4.403297,-5.518539,-6.578461,-7.332562,-7.702192,-7.582977,-7.376856,-6.745349,-6.279895,-5.891862,-5.636418,-5.456323,-5.437006},
        {-1.937289,-1.889079,-1.765709,-1.620800,-1.598110,-1.815613,-2.314443,-3.041183,-3.857777,-4.533446,-4.931544,-4.962571,-4.717069,-4.357935,-3.971281,-3.646312,-3.422461,-3.269044,-3.231471},
        {-1.126412,-1.103440,-1.049199,-0.969714,-0.917898,-0.962176,-1.182409,-1.566237,-2.065834,-2.552771,-2.884909,-2.977707,-2.811758,-2.629199,-2.355800,-2.118920,-1.949860,-1.834291,-1.800638}
    };
    const float fc_ref[nRho][nTheta] = { /* testRhoCoeffs_f_c */
        {546.421361,425.254555,444.459221,973.709942,1699.786659,2726.090988,3292.439286,4053.852568,4341.862362,4533.682290,4817.707394,4694.993281,4748.459382,4870.501769,5102.893938,5162.638615,5132.204425,5122.829452,5416.913833},
        {426.287912,404.714442,414.615537,637.649598,1160.361962,2178.519781,2960.158333,3873.483410,4241.757848,4475.191240,4640.427624,4625.316514,4679.155327,4806.412033,4946.024583,5105.292687,5130.221346,5135.618382,5358.932615},
        {372.413568,366.887356,366.701592,418.485394,614.092293,1171.701286,2086.975839,3285.887381,3958.715408,4319.557045,4508.344720,4548.575456,4580.908156,4723.304878,4833.747607,5041.132710,5171.711517,5204.174946,5368.723274},
        {331.450650,330.782160,328.263025,339.331019,378.911780,520.341481,1019.403561,2260.279989,3427.141206,4059.058152,4369.357111,4500.541951,4510.215481,4670.641161,4760.980634,5041.448465,5252.603541,5319.084669,5475.690143},
        {309.217148,309.337516,308.433454,312.240168,320.445060,356.143810,530.098653,1434.538907,2809.704701,3790.796603,4240.156702,4471.666599,4473.992083,4649.068401,4715.006822,5047.396206,5322.000678,5416.361327,5593.521239}
    };
    
    /* params to test */
    float g0, gInf, fc;
    
    for(int ri = 0; ri < nRho; ri++){
        float r = rho[ri];
        for(int ti = 0; ti < nTheta; ti++){
            calcHighShelfParams(ti, r, &g0, &gInf, &fc);
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance, g0_ref[ri][ti], g0);
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance, gInf_ref[ri][ti], gInf);
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance_fc, fc_ref[ri][ti], fc);
        }
    }
}

// Parameter interpolation is implicitly checked in test__dvf_calcHighShelfCoeffs
// TODO: consider making a more granular test to just check the interpolation function?
void test__dvf_interpHighShelfParams(void){
    /* interpHighShelfCoeffs() calls calcHighShelfParams() twice to generate the high
     shelf parameters for the nearests thetas in the lookup table. Those parameters
     are subsequently interpolated. So the success of interpHighShelfCoeffs() relies
     first on the calcHighShelfParams(), so that should be tested first. */
    
    /* setup */
    const float acceptedTolerance = 0.0001f;  // TODO: revisit these thresholds
    const float acceptedTolerance_frq = 0.01f;
    const int nTheta = 6;
    const float theta[nTheta] = {0.000000,2.300000,47.614000,98.600000,166.200000,180.000000};
    const int nRho = 5;
    const float rho[nRho] = {1.150000,1.250000,1.570000,2.381000,3.990000};
    const float iG0_ref[nRho][nTheta] = { /* testShelfParamsInterp_iG_0 */
        {22.670282,21.531200,2.814473,-5.412009,-8.989264,-9.089385},
        {18.295933,17.655270,3.178890,-4.810981,-8.364464,-8.467470},
        {11.937032,11.742982,3.538333,-3.463974,-6.856924,-6.961620},
        {6.676990,6.625110,3.023452,-1.880613,-4.729220,-4.823625},
        {3.628860,3.607114,2.019588,-0.892461,-2.938593,-3.010937}
    };
    const float iGInf_ref[nRho][nTheta] = { /* testShelfParamsInterp_iG_inf */
        {-4.643651,-4.547427,-6.154277,-11.120993,-8.603536,-8.574359},
        {-4.128221,-4.060667,-5.063987,-9.950522,-7.573856,-7.518260},
        {-3.094135,-3.064137,-3.266476,-7.650444,-5.524759,-5.437006},
        {-1.937289,-1.926201,-1.763717,-4.875811,-3.327342,-3.231471},
        {-1.126412,-1.121129,-0.951611,-2.838410,-1.878207,-1.800638}
    };
    const float iFc_ref[nRho][nTheta] = { /* testShelfParamsInterp_if_c */
        {546.421361,518.552996,2481.214775,4777.943880,5126.391942,5416.913833},
        {426.287912,421.326014,1935.587325,4617.294530,5133.567508,5358.932615},
        {372.413568,371.142539,1038.655780,4481.914446,5191.838843,5368.723274},
        {331.450650,331.296897,486.596354,4325.915257,5293.821840,5475.690143},
        {309.217148,309.244833,347.626088,4177.246288,5380.504281,5593.521239}
    };
    /* High shelf parameters to be tested */
    float iG0, iGInf, iFc;

    for(int ri = 0; ri < nRho; ri++){
        float r = rho[ri];
        for(int ti = 0; ti < nTheta; ti++){
            interpHighShelfParams(theta[ti], r, &iG0, &iGInf, &iFc);
            
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance, iG0, iG0_ref[ri][ti]);
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance, iGInf, iGInf_ref[ri][ti]);
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance_frq, iFc, iFc_ref[ri][ti]);
        }
    }
}

void test__dvf_calcIIRCoeffs(void){
    /* setup */
    const float acceptedTolerance = 0.00001f; // TODO: revisit these thresholds
    const int nTheta = 6;
    const float theta[nTheta] = {0.000000,2.300000,47.614000,98.600000,166.200000,180.000000};
    const int nRho = 5;
    const float rho[nRho] = {1.150000,1.250000,1.570000,2.381000,3.990000};
    const float fs = 44100;
    const float b0_ref[nRho][nTheta] = { /* testIIRCoefs_b0 */
        {8.093308,7.170404,0.737524,0.183757,0.159702,0.159830},
        {5.167212,4.835967,0.850704,0.221158,0.190276,0.190328},
        {2.789474,2.737040,1.054606,0.325505,0.276782,0.276874},
        {1.733775,1.725607,1.163241,0.512548,0.434778,0.434961},
        {1.337404,1.334856,1.133677,0.696375,0.608321,0.608423}
    };
    const float b1_ref[nRho][nTheta] = { /* testIIRCoefs_b1 */
        {-7.486541,-6.659294,-0.513980,-0.087641,-0.071358,-0.067482},
        {-4.862536,-4.554047,-0.643374,-0.108602,-0.084902,-0.081287},
        {-2.645257,-2.596005,-0.909046,-0.163758,-0.122126,-0.118022},
        {-1.653774,-1.646019,-1.085268,-0.265035,-0.188078,-0.181504},
        {-1.279744,-1.277301,-1.078874,-0.369490,-0.258703,-0.247912}
    };
    const float a1_ref[nRho][nTheta] = { /* testIIRCoefs_a1 */
        {-0.955382,-0.957150,-0.838326,-0.820775,-0.751319,-0.737038},
        {-0.962928,-0.963071,-0.856214,-0.804153,-0.723970,-0.710957},
        {-0.963511,-0.963510,-0.903144,-0.758990,-0.659425,-0.645941},
        {-0.962911,-0.962881,-0.944948,-0.692654,-0.574765,-0.558344},
        {-0.962031,-0.962005,-0.956566,-0.637742,-0.509631,-0.490124}
    };
    
    float b0, b1, a1;       /* IIR coeffs to be tested */
    float iG0, iGInf, iFc;

    for(int ri = 0; ri < nRho; ri++){
        float r = rho[ri];
        for(int ti = 0; ti < nTheta; ti++){
            interpHighShelfParams(theta[ti], r, &iG0, &iGInf, &iFc);
            calcIIRCoeffs(iG0, iGInf, iFc, fs, &b0, &b1, &a1);
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance, b0, b0_ref[ri][ti]);
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance, b1, b1_ref[ri][ti]);
            TEST_ASSERT_FLOAT_WITHIN(acceptedTolerance, a1, a1_ref[ri][ti]);
        }
    }
}
