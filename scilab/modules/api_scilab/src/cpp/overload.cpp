/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "stdarg.h"
#include "localization.h"
#include "os_swprintf.h"
#include "sci_malloc.h"
}

#include "callable.hxx"
#include "overload.hxx"
#include "context.hxx"
#include "scilabexception.hxx"

std::wstring Overload::buildOverloadName(std::wstring _stFunctionName, types::typed_list &in, int _iRetCount, bool _isOperator)
{
    switch (in.size())
    {
        case 0 :
            return L"%_" + _stFunctionName;
        case 2:
            if (_isOperator)
            {
                return L"%" + in[0]->getShortTypeStr() + L"_" + _stFunctionName + L"_" + in[1]->getShortTypeStr();
            }
        case 1:
            return L"%" + in[0]->getShortTypeStr() + L"_" + _stFunctionName;
        default :
            throw ast::ScilabError(L"Don't know how to overload " + _stFunctionName, 246, *new Location());
    }
    return _stFunctionName;
}

types::Function::ReturnValue Overload::generateNameAndCall(std::wstring _stFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, ast::ConstVisitor *_execMe, bool _isOperator)
{
    return call(buildOverloadName(_stFunctionName, in, _iRetCount, _isOperator), in, _iRetCount, out, _execMe, _isOperator);
}

types::Function::ReturnValue Overload::call(std::wstring _stOverloadingFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, ast::ConstVisitor *_execMe, bool _isOperator)
{
    types::InternalType *pIT = symbol::Context::getInstance()->get(symbol::Symbol(_stOverloadingFunctionName));

    if (pIT == NULL || pIT->isCallable() == false)
    {
        char pstError1[512];
        char pstError2[512];
        char *pstFuncName = wide_string_to_UTF8(_stOverloadingFunctionName.c_str());
        wchar_t* pwstError = NULL;
        if (_isOperator)
        {
            sprintf(pstError2, _("  check or define function %s for overloading.\n"), pstFuncName);
            sprintf(pstError1, "%s%s", _("Undefined operation for the given operands.\n"), pstError2);
            pwstError = to_wide_string(pstError1);
            std::wstring wstError(pwstError);
            FREE(pwstError);
            FREE(pstFuncName);
            throw ast::ScilabError(wstError, 144, *new Location());
        }
        else
        {
            sprintf(pstError2, _("  check arguments or define function %s for overloading.\n"), pstFuncName);
            sprintf(pstError1, "%s%s", _("Function not defined for given argument type(s),\n"), pstError2);
            pwstError = to_wide_string(pstError1);
            std::wstring wstError(pwstError);
            FREE(pwstError);
            FREE(pstFuncName);
            throw ast::ScilabError(wstError, 246, *new Location());
        }
    }
    types::Callable *pCall = pIT->getAs<types::Callable>();
    try
    {
        types::optional_list opt;
        return pCall->call(in, opt, _iRetCount, out, _execMe);
    }
    catch (ast::ScilabMessage sm)
    {
        if (pCall->isMacro() || pCall->isMacroFile())
        {
            wchar_t szError[bsiz];
            os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), sm.GetErrorLocation().first_line, pCall->getName().c_str());
            throw ast::ScilabMessage(szError);
        }
        else
        {
            throw sm;
        }
    }
}

std::wstring Overload::getNameFromOper(ast::OpExp::Oper _oper)
{
    switch (_oper)
    {
            /* standard operators */
        case ast::OpExp::plus :
            return std::wstring(L"a");
        case ast::OpExp::unaryMinus :
        case ast::OpExp::minus :
            return std::wstring(L"s");
        case ast::OpExp::times :
            return std::wstring(L"m");
        case ast::OpExp::rdivide :
            return std::wstring(L"r");
        case ast::OpExp::ldivide :
            return std::wstring(L"l");
        case ast::OpExp::power :
            return std::wstring(L"p");
            /* dot operators */
        case ast::OpExp::dottimes :
            return std::wstring(L"x");
        case ast::OpExp::dotrdivide :
            return std::wstring(L"d");
        case ast::OpExp::dotldivide :
            return std::wstring(L"q");
        case ast::OpExp::dotpower :
            return std::wstring(L"j");
            /* Kron operators */
        case ast::OpExp::krontimes :
            return std::wstring(L"k");
        case ast::OpExp::kronrdivide :
            return std::wstring(L"y");
        case ast::OpExp::kronldivide :
            return std::wstring(L"z");
            /* Control Operators ??? */
        case ast::OpExp::controltimes :
            return std::wstring(L"u");
        case ast::OpExp::controlrdivide :
            return std::wstring(L"v");
        case ast::OpExp::controlldivide :
            return std::wstring(L"w");
        case ast::OpExp::eq :
            return std::wstring(L"o");
        case ast::OpExp::ne :
            return std::wstring(L"n");
        case ast::OpExp::lt :
            return std::wstring(L"1");
        case ast::OpExp::le :
            return std::wstring(L"3");
        case ast::OpExp::gt :
            return std::wstring(L"2");
        case ast::OpExp::ge :
            return std::wstring(L"4");
        case ast::OpExp::logicalAnd :
            return std::wstring(L"h");
        case ast::OpExp::logicalOr :
            return std::wstring(L"g");
        case ast::OpExp::logicalShortCutAnd :
            return std::wstring(L"h");
        case ast::OpExp::logicalShortCutOr :
            return std::wstring(L"g");
        default :
            return std::wstring(L"???");
    }
}