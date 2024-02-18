package woo.app.suppliers;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;
import woo.app.exceptions.UnauthorizedSupplierException;
import woo.app.exceptions.UnknownSupplierKeyException;
import woo.app.suppliers.Message;
//FIXME import other classes
import woo.exceptions.BadEntryException;

/**
 * Show all transactions for specific supplier.
 */
public class DoShowSupplierTransactions extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _supplierid; 

  public DoShowSupplierTransactions(Storefront receiver) {
    super(Label.SHOW_SUPPLIER_TRANSACTIONS, receiver);
    //FIXME init input fields
    _supplierid = _form.addStringInput(Message.requestSupplierKey());
  }

  @Override
  public void execute() throws DialogException {
    //FIXME implement command
	  _form.parse();
	  try {
		  _display.popup(_receiver.showSupplierTransactions(_supplierid.value()).toString());
	  }
	  catch(BadEntryException e) {
		  throw new UnauthorizedSupplierException(_supplierid.value());
	  }
  }

}
