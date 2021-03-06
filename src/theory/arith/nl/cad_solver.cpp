/*********************                                                        */
/*! \file cad_solver.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of new non-linear solver
 **/

#include "theory/arith/nl/cad_solver.h"

#ifdef CVC4_POLY_IMP
#include <poly/polyxx.h>
#endif

#include "inference.h"
#include "theory/arith/nl/cad/cdcac.h"
#include "theory/arith/nl/poly_conversion.h"
#include "util/poly_util.h"

namespace CVC4 {
namespace theory {
namespace arith {
namespace nl {

CadSolver::CadSolver(TheoryArith& containing, NlModel& model)
    : d_foundSatisfiability(false), d_containing(containing), d_model(model)
{
  d_ranVariable =
      NodeManager::currentNM()->mkSkolem("__z",
                                         NodeManager::currentNM()->realType(),
                                         "",
                                         NodeManager::SKOLEM_EXACT_NAME);
}

CadSolver::~CadSolver() {}

void CadSolver::initLastCall(const std::vector<Node>& assertions)
{
#ifdef CVC4_POLY_IMP
  if (Trace.isOn("nl-cad"))
  {
    Trace("nl-cad") << "CadSolver::initLastCall" << std::endl;
    Trace("nl-cad") << "* Assertions: " << std::endl;
    for (const Node& a : assertions)
    {
      Trace("nl-cad") << "  " << a << std::endl;
    }
  }
  // store or process assertions
  d_CAC.reset();
  for (const Node& a : assertions)
  {
    d_CAC.getConstraints().addConstraint(a);
  }
  d_CAC.computeVariableOrdering();
#else
  Warning() << "Tried to use CadSolver but libpoly is not available. Compile "
               "with --poly."
            << std::endl;
#endif
}

std::vector<NlLemma> CadSolver::checkFull()
{
#ifdef CVC4_POLY_IMP
  std::vector<NlLemma> lems;
  auto covering = d_CAC.getUnsatCover();
  if (covering.empty())
  {
    d_foundSatisfiability = true;
    Trace("nl-cad") << "SAT: " << d_CAC.getModel() << std::endl;
  }
  else
  {
    d_foundSatisfiability = false;
    auto mis = collectConstraints(covering);
    Trace("nl-cad") << "Collected MIS: " << mis << std::endl;
    auto* nm = NodeManager::currentNM();
    for (auto& n : mis)
    {
      n = n.negate();
    }
    Assert(!mis.empty()) << "Infeasible subset can not be empty";
    if (mis.size() == 1)
    {
      lems.emplace_back(mis.front(), Inference::CAD_CONFLICT);
    }
    else
    {
      lems.emplace_back(nm->mkNode(Kind::OR, mis), Inference::CAD_CONFLICT);
    }
    Trace("nl-cad") << "UNSAT with MIS: " << lems.back().d_lemma << std::endl;
  }
  return lems;
#else
  Warning() << "Tried to use CadSolver but libpoly is not available. Compile "
               "with --poly."
            << std::endl;
  return {};
#endif
}

bool CadSolver::constructModelIfAvailable(std::vector<Node>& assertions)
{
#ifdef CVC4_POLY_IMP
  if (!d_foundSatisfiability)
  {
    return false;
  }
  assertions.clear();
  for (const auto& v : d_CAC.getVariableOrdering())
  {
    Node variable = d_CAC.getConstraints().varMapper()(v);
    Node value = value_to_node(d_CAC.getModel().get(v), d_ranVariable);
    if (value.isConst())
    {
      d_model.addCheckModelSubstitution(variable, value);
    }
    else
    {
      d_model.addCheckModelWitness(variable, value);
    }
    Trace("nl-cad") << "-> " << v << " = " << value << std::endl;
  }
  return true;
#else
  Warning() << "Tried to use CadSolver but libpoly is not available. Compile "
               "with --poly."
            << std::endl;
  return false;
#endif
}

}  // namespace nl
}  // namespace arith
}  // namespace theory
}  // namespace CVC4
