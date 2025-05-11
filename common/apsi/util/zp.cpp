//
// Created by bush on 29/01/18.
//

#include "zp.h"

#define LEFT(X) (2 * X + 1)
#define RIGHT(X) (2 * X + 2)
#define PAPA(X) ((X - 1) / 2)

void print_poly(ZZ_pX &P)
{
    long degree = deg(P);
    if (-1 == degree) {
        cout << "0";
        return;
    }
    for (long i = 0; i <= degree; i++) {
        cout << coeff(P, i);
        if (i == 1)
            cout << "X";
        else if (i > 1)
            cout << "X^" << i;
        if (i < degree) {
            cout << " + ";
        }
    }
    //    cout << endl << "random poly:" << endl << P << endl;
}

void build_tree(ZZ_pX *tree, ZZ_p *points, unsigned int tree_size)
{
    // std::cout << "tree_size " << tree_size << std::endl;
    ZZ_p negated;
    int degree = (tree_size - 1) / 2;
    int i = 2 * degree;
    unsigned int point_index;
    for (; i >= degree; i--) {
        point_index = i - degree;
        NTL::negate(negated, points[point_index]);
        SetCoeff(tree[i], 0, negated);
        SetCoeff(tree[i], 1, 1);
    }

    for (; i > 0; i--) {
        tree[i] = tree[LEFT(i)] * tree[RIGHT(i)];
    }
    tree[0] = tree[1] * tree[2];
}

void test_tree(ZZ_pX &final_polynomial, ZZ_p *points, unsigned int npoints)
{
    ZZ_p result;
    bool error = false;
    for (unsigned int i = 0; i < npoints; i++) {
        result = eval(final_polynomial, points[i]);
        if (0 != result) {
            cout << "FATAL ERROR: polynomials tree is incorrect!" << endl;
            error = true;
            break;
        }
    }
    if (!error)
        cout << "polynomials tree is correct." << endl;
}

void evaluate(ZZ_pX &P, ZZ_pX *tree, ZZ_pX *reminders, unsigned int tree_size, ZZ_p *results)
{
    reminders[0] = P % tree[0];

    unsigned int i = 1;
    for (; i < tree_size / 2; i++) {
        //        cout << "i="<<i <<": ";
        reminders[i] = reminders[PAPA(i)] % tree[i];
    }

    unsigned int result_index;
    for (; i < tree_size; i++) {
        reminders[i] = reminders[PAPA(i)] % tree[i];
        result_index = i - (tree_size - 1) / 2;
        results[result_index] = coeff(reminders[i], 0);
    }
}

void test_evaluate(ZZ_pX &P, ZZ_p *points, ZZ_p *results, unsigned int npoints)
{
    bool error = false;
    for (unsigned int i = 0; i < npoints; i++) {
        ZZ_p y = eval(P, points[i]);
        if (y != results[i]) {
            cout << "y=" << y << " and results[i]=" << results[i] << endl;
            error = true;
        }
    }
    if (error)
        cout << "ERROR: evaluation results do not match real evaluation!" << endl;
    else
        cout << "All evaluation results computed correctly!" << endl;
}

void multipoint_evaluate_zp(ZZ_pX &P, ZZ_p *x, ZZ_p *y, long degree)
{
    //    cout << "P:" <<endl; print_poly(P); cout << endl;
    // we want to evaluate P on 'degree+1' values.
    ZZ_pX *p_tree = new ZZ_pX[degree * 2 + 1];
    steady_clock::time_point begin1 = steady_clock::now();
    build_tree(p_tree, x, degree * 2 + 1);
    steady_clock::time_point end1 = steady_clock::now();
    //    test_tree(p_tree[0], x, degree+1);

    ZZ_pX *reminders = new ZZ_pX[degree * 2 + 1];
    steady_clock::time_point begin2 = steady_clock::now();
    evaluate(P, p_tree, reminders, degree * 2 + 1, y);
    chrono::steady_clock::time_point end2 = steady_clock::now();
    //    test_evaluate(P,x,y,degree+1);

    cout << "Building tree: " << duration_cast<milliseconds>(end1 - begin1).count() << " ms" << endl;
    cout << "Evaluating points: " << duration_cast<milliseconds>(end2 - begin2).count() << " ms" << endl;
    cout << "Total: "
         << duration_cast<milliseconds>(end1 - begin1).count() + duration_cast<milliseconds>(end2 - begin2).count()
         << " ms" << endl;
}

void test_multipoint_eval_zp(ZZ prime, long degree)
{
    // init underlying prime field
    ZZ_p::init(ZZ(prime));

    // the given polynomial
    ZZ_pX P;
    random(P, degree + 1);
    SetCoeff(P, degree, random_ZZ_p());

    // evaluation points:
    ZZ_p *x = new ZZ_p[degree + 1];
    ZZ_p *y = new ZZ_p[degree + 1];

    for (unsigned int i = 0; i <= degree; i++) {
        random(x[i]);
    }

    multipoint_evaluate_zp(P, x, y, degree);
}

/*
 * expects an "empty" polynomial 'resultP'
 */
void iterative_interpolate_zp(ZZ_pX &resultP, ZZ_pX *temp, ZZ_p *y, ZZ_p *a, ZZ_pX *M, unsigned int tree_size)
{
    unsigned int i = tree_size - 1;
    ZZ_p inv_a;
    unsigned int y_index;
    for (; i >= tree_size / 2; i--) {
        y_index = i - (tree_size - 1) / 2;

        inv(inv_a, a[y_index]); // inv_a = 1/a[y_index]
        SetCoeff(temp[i], 0, y[y_index] * inv_a);
    }

    for (; i > 0; i--) {
        temp[i] = temp[LEFT(i)] * M[RIGHT(i)] + temp[RIGHT(i)] * M[LEFT(i)];
    }

    resultP = temp[LEFT(0)] * M[RIGHT(0)] + temp[RIGHT(0)] * M[LEFT(0)];
}

void interpolate_zp(ZZ_pX &resultP, ZZ_p *x, ZZ_p *y, long degree)
{
    system_clock::time_point begin[5];
    system_clock::time_point end[5];

    // we first build the tree of the super moduli
    ZZ_pX *M = new ZZ_pX[degree * 2 + 1];
    begin[1] = system_clock::now();
    build_tree(M, x, degree * 2 + 1);
    end[1] = system_clock::now();
    //    test_tree(M[0], x, degree+1);

    // we construct a preconditioned global structure for the a_k for all
    // 1<=k<=(degree+1)
    ZZ_p *a = new ZZ_p[degree + 1];
    ZZ_pX D;
    begin[2] = system_clock::now();
    diff(D, M[0]);
    end[2] = system_clock::now();

    // evaluate d(x) to obtain the results in the array a
    ZZ_pX *reminders = new ZZ_pX[degree * 2 + 1];
    begin[3] = system_clock::now();
    evaluate(D, M, reminders, degree * 2 + 1, a);
    end[3] = system_clock::now();
    //    test_evaluate(D,x,a,degree+1);

    // now we can apply the formula
    ZZ_pX *temp = new ZZ_pX[degree * 2 + 1];
    begin[4] = system_clock::now();
    iterative_interpolate_zp(resultP, temp, y, a, M, degree * 2 + 1);
    end[4] = system_clock::now();

    delete[] M;
    delete[] a;
    delete[] reminders;
    delete[] temp;
    // cout << "Building tree: " << duration_cast<milliseconds>(end[1] - begin[1]).count() << " ms" << endl;
    // cout << "Differentiate: " << duration_cast<milliseconds>(end[2] - begin[2]).count() << " ms" << endl;
    // cout << "Evaluate diff: " << duration_cast<milliseconds>(end[3] - begin[3]).count() << " ms" << endl;
    // cout << "Interpolation: " << duration_cast<milliseconds>(end[4] - begin[4]).count() << " ms" << endl;
    // cout << "Total: "
    //      << duration_cast<milliseconds>(end[1] - begin[1] + end[2] - begin[2] + end[3] - begin[3] + end[4] -
    //      begin[4])
    //             .count()
    //      << " ms" << endl;
}

void test_interpolation_result_zp(ZZ_pX &P, ZZ_p *x, ZZ_p *y, long degree)
{
    cout << "Testing result polynomial" << endl;
    ZZ_p res;
    for (long i = 0; i < degree + 1; i++) {
        eval(res, P, x[i]);
        if (res != y[i]) {
            cout << "Error! x = " << x[i] << ", y = " << y[i] << ", res = " << res << endl;
            return;
        }
    }
    cout << "Polynomial is interpolated correctly!" << endl;
}

void test_interpolate_zp(ZZ prime, long degree)
{
    // init underlying prime field
    ZZ_p::init(ZZ(prime));

    // interpolation points:
    ZZ_p *x = new ZZ_p[degree + 1];
    ZZ_p *y = new ZZ_p[degree + 1];
    for (unsigned int i = 0; i <= degree; i++) {
        random(x[i]);
        random(y[i]);
    }

    ZZ_pX P;
    interpolate_zp(P, x, y, degree);
    cout << "P: ";
    print_poly(P);
    cout << endl;
    test_interpolation_result_zp(P, x, y, degree);
}