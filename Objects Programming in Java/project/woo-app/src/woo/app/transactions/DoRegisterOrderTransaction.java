package woo.app.transactions;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Form;
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;
import woo.app.exceptions.UnauthorizedSupplierException;
//FIXME import other classes
import woo.exceptions.BadEntryException;

/**
 * Register order.
 */
public class DoRegisterOrderTransaction extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _supplierId;
	private Input<String> _productId;
	private Input<Integer> _amount;
	private Input<Boolean> _boolean;
	private Boolean _b;
	

  public DoRegisterOrderTransaction(Storefront receiver) {
    super(Label.REGISTER_ORDER_TRANSACTION, receiver);
    //FIXME init input fields
    _supplierId = _form.addStringInput(Message.requestSupplierKey());
    _productId = _form.addStringInput(Message.requestProductKey());
    _amount = _form.addIntegerInput(Message.requestAmount());
    _boolean = _form.addBooleanInput(Message.requestMore());

    
  }
  
  

  @Override
  public final void execute() throws DialogException {
    //FIXME implement command
	  try {
		  _form.parse();
		  _receiver.registerOrder(_supplierId.value(), _productId.value(), _amount.value());
		  _b = _boolean.value();
		  while(_b == true) {
			  _form.clear();
			  _productId = _form.addStringInput(Message.requestProductKey());
			  _amount = _form.addIntegerInput(Message.requestAmount());
			  _boolean = _form.addBooleanInput(Message.requestMore());
			  _form.parse();
			  _receiver.registerOrderAdded(_productId.value(), _amount.value());
			  //_receiver.registerOrder(_supplierId.value(), _productId.value(), _amount.value());
			  _b = _boolean.value();
			  _form.clear();
			  
		  }
		  _supplierId = _form.addStringInput(Message.requestSupplierKey());
		  _productId = _form.addStringInput(Message.requestProductKey());
		  _amount = _form.addIntegerInput(Message.requestAmount());
		  _boolean = _form.addBooleanInput(Message.requestMore());
		  
	  } catch (BadEntryException e) {
			// TODO Auto-generated catch block
			throw new UnauthorizedSupplierException(_supplierId.value());
	  }
  }	

}
