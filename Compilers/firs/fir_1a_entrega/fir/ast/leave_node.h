#ifndef __FIR_AST_LEAVE_NODE_H__
#define __FIR_AST_LEAVE_NODE_H__

#include <cdk/ast/basic_node.h>

namespace fir {

  /**
   * Class for describing while-cycle nodes.
   */
  class leave_node: public cdk::basic_node {
    int _level;

  public:
    inline leave_node(int lineno, int level = 1) :
        cdk::basic_node(lineno), _level(level) {
    }

  public:
    public:
      int level() const {
        return _level;
      }


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_leave_node(this, level);
    }

  };

} // fir

#endif
