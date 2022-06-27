//===----- CFLAlias.h -- CFL Alias Analysis Client--------------//
//
//                     SVF: Static Value-Flow Analysis
//
// Copyright (C) <2013->  <Yulei Sui>
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//

/*
 * CFLAlias.h
 *
 *  Created on: March 5, 2022
 *      Author: Yulei Sui
 */


#include "CFL/CFLSolver.h"
#include "CFL/CFGNormalizer.h"
#include "CFL/GrammarBuilder.h"
#include "CFL/CFLGraphBuilder.h"
#include "CFL/CFLGramGraphChecker.h"
#include "MemoryModel/PointerAnalysis.h"
#include "Graphs/ConsG.h"
#include "Util/Options.h"

namespace SVF
{

class CFLAlias : public PointerAnalysis
{

public:
    CFLAlias(SVFIR* ir) : PointerAnalysis(ir, PointerAnalysis::CFLFICI_WPA), svfir(ir), graph(nullptr), grammar(nullptr), solver(nullptr)
    {
    }

    /// Destructor
    virtual ~CFLAlias()
    {
        delete solver;
    }

    /// Start Analysis here (main part of pointer analysis).
    virtual void analyze();

    /// Interface exposed to users of our pointer analysis, given Value infos
    virtual AliasResult alias(const Value* v1, const Value* v2)
    {
        NodeID n1 = svfir->getValueNode(v1);
        NodeID n2 = svfir->getValueNode(v2);
        return alias(n1,n2);
    }

    /// Interface exposed to users of our pointer analysis, given PAGNodeID
    virtual AliasResult alias(NodeID node1, NodeID node2)
    {
        if(graph->hasEdge(graph->getGNode(node1), graph->getGNode(node2), graph->startKind))
            return AliasResult::MayAlias;
        else
            return AliasResult::NoAlias;
    }

    /// Get points-to targets of a pointer.
    virtual const PointsTo& getPts(NodeID ptr)
    {
        /// Check Outgoing edge Dst of ptr
        //PointsTo * ps;
        abort(); // to be implemented
    }

    /// Given an object, get all the nodes having whose pointsto contains the object
    virtual const NodeSet& getRevPts(NodeID nodeId)
    {
        /// Check Outgoing flowtobar edge dst of ptr
        abort(); // to be implemented
    }

private:
    SVFIR* svfir;
    CFLGraph* graph;
    CFLGrammar* grammar;
    CFLSolver *solver;
};

}