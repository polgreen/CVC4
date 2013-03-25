/*********************                                                        */
/*! \file bv_subtheory_inequality.cpp
 ** \verbatim
 ** Original author: lianah
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 prototype.
 ** Copyright (c) 2009-2012  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Algebraic solver.
 **
 ** Algebraic solver.
 **/

#include "theory/bv/bv_subtheory_inequality.h"
#include "theory/bv/theory_bv.h"
#include "theory/bv/theory_bv_utils.h"
#include "theory/model.h"

using namespace std;
using namespace CVC4;
using namespace CVC4::context;
using namespace CVC4::theory;
using namespace CVC4::theory::bv;
using namespace CVC4::theory::bv::utils;

bool InequalitySolver::check(Theory::Effort e) {
  Debug("bv-subtheory-inequality") << "InequalitySolveR::check("<< e <<")\n"; 
  bool ok = true; 
  while (!done() && ok) {
    TNode fact = get();
    Debug("bv-subtheory-inequality") << "  "<< fact <<"\n"; 
    if (fact.getKind() == kind::EQUAL) {
      TNode a = fact[0];
      TNode b = fact[1];
      ok = d_inequalityGraph.addInequality(a, b, false, fact);
      if (ok)
        ok = d_inequalityGraph.addInequality(b, a, false, fact); 
    } else if (fact.getKind() == kind::NOT && fact[0].getKind() == kind::EQUAL) {
      TNode a = fact[0][0];
      TNode b = fact[0][1];
      ok = d_inequalityGraph.addDisequality(a, b, fact);
    }
    if (fact.getKind() == kind::NOT && fact[0].getKind() == kind::BITVECTOR_ULE) {
      TNode a = fact[0][1];
      TNode b = fact[0][0]; 
      ok = d_inequalityGraph.addInequality(a, b, true, fact);
    } else if (fact.getKind() == kind::NOT && fact[0].getKind() == kind::BITVECTOR_ULT) {
      TNode a = fact[0][1];
      TNode b = fact[0][0]; 
      ok = d_inequalityGraph.addInequality(a, b, false, fact);
    } else if (fact.getKind() == kind::BITVECTOR_ULT) {
      TNode a = fact[0];
      TNode b = fact[1]; 
      ok = d_inequalityGraph.addInequality(a, b, true, fact);
    } else if (fact.getKind() == kind::BITVECTOR_ULE) {
      TNode a = fact[0];
      TNode b = fact[1]; 
      ok = d_inequalityGraph.addInequality(a, b, false, fact);
    }
  }
  
  if (!ok) {
    std::vector<TNode> conflict;
    d_inequalityGraph.getConflict(conflict); 
    d_bv->setConflict(utils::flattenAnd(conflict));
    return false; 
  }

  // make sure all the disequalities we didn't split on are still satisifed
  // and split on the ones that are not
  d_inequalityGraph.checkDisequalities();

  // send out any lemmas
  std::vector<Node> lemmas;
  d_inequalityGraph.getNewLemmas(lemmas); 
  for(unsigned i = 0; i < lemmas.size(); ++i) {
    d_bv->lemma(lemmas[i]); 
  }
  return true; 
}

EqualityStatus InequalitySolver::getEqualityStatus(TNode a, TNode b) {
  Node a_lt_b = utils::mkNode(kind::BITVECTOR_ULT, a, b);
  Node b_lt_a = utils::mkNode(kind::BITVECTOR_ULT, b, a);

  // if an inequality containing the terms has been asserted then we know
  // the equality is false
  if (d_assertionSet.contains(a_lt_b) || d_assertionSet.contains(b_lt_a)) {
    return EQUALITY_FALSE; 
  }
  
  if (!d_inequalityGraph.hasValueInModel(a) ||
      !d_inequalityGraph.hasValueInModel(b)) {
    return EQUALITY_UNKNOWN; 
  }

  // TODO: check if this disequality is entailed by inequalities via transitivity
  
  BitVector a_val = d_inequalityGraph.getValueInModel(a);
  BitVector b_val = d_inequalityGraph.getValueInModel(b);
  
  if (a_val == b_val) {
    return EQUALITY_TRUE_IN_MODEL; 
  } else {
    return EQUALITY_FALSE_IN_MODEL; 
  }
}

void InequalitySolver::assertFact(TNode fact) {
  d_assertionQueue.push_back(fact);
  d_assertionSet.insert(fact); 
}

void InequalitySolver::explain(TNode literal, std::vector<TNode>& assumptions) {
  Assert (false); 
}

void InequalitySolver::propagate(Theory::Effort e) {
  Assert (false); 
}

