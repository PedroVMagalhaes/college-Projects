#ifndef __FIR_AST_FUNCTION_DEFINITION_NODE_H__
#define __FIR_AST_FUNCTION_DEFINITION_NODE_H__

#include <string>
#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>

namespace fir {

  /**
   * Class for describing function_definition nodes.
   */
  class function_definition_node: public cdk::typed_node {
    int _qualifier;
    std::string _identifier;
    cdk::sequence_node *_arguments;
    cdk::basic_node *_block;

  public:
    function_definition_node(int lineno, int qualifier, const std::string &identifier, cdk::sequence_node *arguments,
                             cdk::basic_node *block) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments), _block(block) {
      type(cdk::primitive_type::create(0, cdk::TYPE_VOID));
    }

    function_definition_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> funType, const std::string &identifier,
                             cdk::sequence_node *arguments, cdk::basic_node *block) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments), _block(block) {
      type(funType);
    }

  public:
    int qualifier() {
      return _qualifier;
    }
    const std::string& identifier() const {
      return _identifier;
    }
    cdk::sequence_node* arguments() {
      return _arguments;
    }
    cdk::typed_node* argument(size_t ax) {
      return dynamic_cast<cdk::typed_node*>(_arguments->node(ax));
    }
    cdk::basic_node * block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // fir

#endif
