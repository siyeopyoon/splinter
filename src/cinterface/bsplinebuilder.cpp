/*
 * This file is part of the SPLINTER library.
 * Copyright (C) 2012 Bjarne Grimstad (bjarne.grimstad@gmail.com).
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bsplinebuilder.h"
#include "cinterface/cinterface.h"
#include "cinterface/builder.h"
#include "cinterface/utilities.h"

using namespace SPLINTER;

extern "C"
{

splinter_obj_ptr splinter_bspline_builder_init(splinter_obj_ptr datatable_ptr)
{
    splinter_obj_ptr bspline_builder_ptr = nullptr;

    try
    {
        DataTable *dataTable = get_datatable(datatable_ptr);
        bspline_builder_ptr = new BSpline::Builder(*dataTable);
        builders.insert(bspline_builder_ptr);
    }
    catch (const Exception &e)
    {
        set_error_string(e.what());
    }

    return bspline_builder_ptr;
}

void splinter_bspline_builder_set_degree(splinter_obj_ptr bspline_builder_ptr, unsigned int *degrees, int n)
{
    auto builder = get_builder<BSpline>(bspline_builder_ptr);
    if(builder != nullptr)
    {
        auto _degrees = get_vector(degrees, n);
        builder->degree(_degrees);
    }
}

void splinter_bspline_builder_set_num_basis_functions(splinter_obj_ptr bspline_builder_ptr, int *num_basis_functions, int n)
{
    auto builder = get_builder<BSpline>(bspline_builder_ptr);
    if(builder != nullptr)
    {
        std::vector<unsigned int> _num_basis_functions((unsigned int) n);
        for (int i = 0; i < n; ++i)
        {
            _num_basis_functions.at(i) = (unsigned int) num_basis_functions[i];
        }
        builder->numBasisFunctions(_num_basis_functions);
    }
}

void splinter_bspline_builder_set_knot_spacing(splinter_obj_ptr bspline_builder_ptr, int knot_spacing)
{
    auto builder = get_builder<BSpline>(bspline_builder_ptr);
    if(builder != nullptr)
    {
        switch (knot_spacing)
        {
            case 0:
                builder->knotSpacing(BSpline::KnotSpacing::SAMPLE);
                break;
            case 1:
                builder->knotSpacing(BSpline::KnotSpacing::EQUIDISTANT);
                break;
            case 2:
                builder->knotSpacing(BSpline::KnotSpacing::EXPERIMENTAL);
                break;
            default:
                set_error_string("Error: Invalid knot spacing!");
                break;
        }
    }
}

void splinter_bspline_builder_set_smoothing(splinter_obj_ptr bspline_builder_ptr, int smoothing)
{
    auto builder = get_builder<BSpline>(bspline_builder_ptr);
    if(builder != nullptr)
    {
        switch (smoothing)
        {
            case 0:
                builder->smoothing(BSpline::Smoothing::NONE);
                break;
            case 1:
                builder->smoothing(BSpline::Smoothing::REGULARIZATION);
                break;
            case 2:
                builder->smoothing(BSpline::Smoothing::PSPLINE);
                break;
            default:
                set_error_string("Error: Invalid smoothing!");
                break;
        }
    }
}

void splinter_bspline_builder_set_lambda(splinter_obj_ptr bspline_builder_ptr, double lambda)
{
    builder_set_lambda<BSpline>(bspline_builder_ptr, lambda);
}

splinter_obj_ptr splinter_bspline_builder_build(splinter_obj_ptr bspline_builder_ptr)
{
    auto func = builder_build<BSpline>(bspline_builder_ptr);

    if (func != nullptr)
    {
        functions.insert(func);
    }

    return func;
}

void splinter_bspline_builder_delete(splinter_obj_ptr bspline_builder_ptr)
{
    builder_delete<BSpline>(bspline_builder_ptr);
}

} // extern "C"
