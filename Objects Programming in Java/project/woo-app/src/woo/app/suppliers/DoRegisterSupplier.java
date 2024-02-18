package woo.app.suppliers;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
//FIXME import other classes
import woo.app.exceptions.DuplicateClientKeyException;
import woo.app.exceptions.DuplicateSupplierKeyException;
import woo.exceptions.BadEntryException;

/**
 * Register supplier.
 */
public class DoRegisterSupplier extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _supplierId;
	private Input<String> _name;
	private Input<String> _address;

  public DoRegisterSupplier(Storefront receiver) {
    super(Label.REGISTER_SUPPLIER, receiver);
    //FIXME init input fields
    _supplierId = _form.addStringInput(Message.requestSupplierKey());
    _name = _form.addStringInput(Message.requestSupplierName());
    _address = _form.addStringInput(Message.requestSupplierAddress());
  }

  @Override
  public void execute() throws DialogException {
    //FIXME implement command
	  _form.parse();
	  try {
		   _receiver.registerSupplier(_supplierId.value(), _name.value(), _address.value());
	  }
	  catch(BadEntryException e) {
		  throw new DuplicateSupplierKeyException(_supplierId.value());
	  }
  }

}
