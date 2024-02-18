package woo.app.suppliers;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;
import woo.app.exceptions.UnknownSupplierKeyException;
import woo.app.suppliers.Message;
//FIXME import other classes
import woo.exceptions.BadEntryException;

/**
 * Enable/disable supplier transactions.
 */
public class DoToggleTransactions extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _supplierid;
	Boolean response;
	
  public DoToggleTransactions(Storefront receiver) {
    super(Label.TOGGLE_TRANSACTIONS, receiver);
    //FIXME init input fields
    _supplierid = _form.addStringInput(Message.requestSupplierKey());
  }

  @Override
  public void execute() throws DialogException {
    //FIXME implement command
	  try {
		  _form.parse();
		  response = _receiver.toggleTransactions(_supplierid.value());
		  if (response == true) {
			  _display.popup(Message.transactionsOn(_supplierid.value()));
			  
		  }
		  else {
			  _display.popup(Message.transactionsOff(_supplierid.value()));
		  }
		  
	  }catch(BadEntryException e) {
		  throw new UnknownSupplierKeyException( _supplierid.value());
	  }
  }

}
