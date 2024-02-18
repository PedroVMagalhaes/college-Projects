#ifndef __FIR_AST_AT_NODE_H__
#define __FIR_AST_AT_NODE_H__

#include <cdk/ast/expression_node.h>

namespace fir {

  /**
   * Class for describing while-cycle nodes.
   */
  class at_node: public cdk::expression_node {
    cdk::expression_node *_condition;
    cdk::basic_node *_block;

  public:
    inline at_node(int lineno) :
        cdk::expression_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_at_node(this, level);
    }

  };

} // fir

#endif
