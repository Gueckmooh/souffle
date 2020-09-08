/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2013, 2014, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file UserDefinedOperator.h
 *
 * Defines a class for evaluating values in the Relational Algebra Machine
 *
 ************************************************************************/

#pragma once

#include "ram/AbstractOperator.h"
#include "ram/Expression.h"
#include "ram/Node.h"
#include "souffle/TypeAttribute.h"
#include "souffle/utility/StreamUtil.h"
#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace souffle {

/**
 * @class RamUserDefinedOperator
 * @brief Operator that represents an extrinsic (user-defined) functor
 */
class RamUserDefinedOperator : public RamAbstractOperator {
public:
    RamUserDefinedOperator(std::string n, std::vector<TypeAttribute> argsTypes, TypeAttribute returnType,
            bool stateful, VecOwn<RamExpression> args)
            : RamAbstractOperator(std::move(args)), name(std::move(n)), argsTypes(std::move(argsTypes)),
              returnType(returnType), stateful(stateful) {
        assert(argsTypes.size() == args.size());
    }

    /** @brief Get operator name */
    const std::string& getName() const {
        return name;
    }

    /** @brief Get types of arguments */
    const std::vector<TypeAttribute>& getArgsTypes() const {
        return argsTypes;
    }

    /** @brief Get return type */
    TypeAttribute getReturnType() const {
        return returnType;
    }

    /** @brief Is functor stateful? */
    bool isStateful() const {
        return stateful;
    }

    RamUserDefinedOperator* clone() const override {
        auto* res = new RamUserDefinedOperator(name, argsTypes, returnType, stateful, {});
        for (auto& cur : arguments) {
            RamExpression* arg = cur->clone();
            res->arguments.emplace_back(arg);
        }
        return res;
    }

protected:
    void print(std::ostream& os) const override {
        os << "@" << name << "_" << argsTypes;
        os << "_" << returnType;
        if (stateful) {
            os << "_stateful";
        }
        os << "("
           << join(arguments, ",", [](std::ostream& out, const Own<RamExpression>& arg) { out << *arg; })
           << ")";
    }

    bool equal(const RamNode& node) const override {
        const auto& other = static_cast<const RamUserDefinedOperator&>(node);
        return RamAbstractOperator::equal(node) && name == other.name && argsTypes == other.argsTypes &&
               returnType == other.returnType && stateful == other.stateful;
    }

    /** Name of user-defined operator */
    const std::string name;

    /** Argument types */
    const std::vector<TypeAttribute> argsTypes;

    /** Return type */
    const TypeAttribute returnType;

    /** Stateful */
    const bool stateful;
};
}  // end of namespace souffle
