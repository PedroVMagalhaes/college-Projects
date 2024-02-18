package woo.app.products;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
//FIXME import other classes

/**
 * Change product price.
 */
public class DoChangePrice extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _productId;
	private Input<Integer> _price;
  
  public DoChangePrice(Storefront receiver) {
    super(Label.CHANGE_PRICE, receiver);
    //FIXME init input fields
    _productId = _form.addStringInput(Message.requestProductKey());
    _price = _form.addIntegerInput(Message.requestPrice());
  }

  @Override
  public final void execute() throws DialogException {
    //FIXME implement command
	  _form.parse();
	  _receiver.changePrice(_productId.value(), _price.value());
  }
}
