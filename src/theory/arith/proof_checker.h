/*********************                                                        */
/*! \file proof_checker.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Alex Ozdemir
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Arithmetic proof checker utility
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__ARITH__PROOF_CHECKER_H
#define CVC4__THEORY__ARITH__PROOF_CHECKER_H

#include "expr/node.h"
#include "expr/proof_checker.h"
#include "expr/proof_node.h"

namespace CVC4 {
namespace theory {
namespace arith {

/** A checker for arithmetic reasoning in proofs */
class ArithProofRuleChecker : public ProofRuleChecker
{
 public:
  ArithProofRuleChecker() {}
  ~ArithProofRuleChecker() {}

  /** Register all rules owned by this rule checker into pc. */
  void registerTo(ProofChecker* pc) override;

 protected:
  /** Return the conclusion of the given proof step, or null if it is invalid */
  Node checkInternal(PfRule id,
                     const std::vector<Node>& children,
                     const std::vector<Node>& args) override;

 private:
  // Returns the rational and the kinds of the bound of the varaible part in
  // relation to it. See normal_form.h for a desription of the kinds and their
  // meaning.
  Rational extractBound(Node n);
  // Returns the rational and the kinds of the bound of the varaible part in
  // relation to it. See normal_form.h for a desription of the kinds and their
  // meaning.
  Kind extractComparisonKind(Node n);
};

}  // namespace arith
}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__ARITH__PROOF_CHECKER_H */
