package woo.app.transactions;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
//FIXME import other classes
import woo.app.transactions.Message;
import woo.exceptions.BadEntryException;
import woo.exceptions.InsufficientStockException;
import woo.app.exceptions.UnavailableProductException;
import woo.app.exceptions.UnknownClientKeyException;

/**
 * Register sale.
 */
public class DoRegisterSaleTransaction extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _clientId;
	private Input<Integer> _deadline;
	private Input<String> _productId;
	private Input<Integer> _amount;
	
  public DoRegisterSaleTransaction(Storefront receiver) {
    super(Label.REGISTER_SALE_TRANSACTION, receiver);
    //FIXME init input fields
    _clientId = _form.addStringInput(Message.requestClientKey());
    _deadline = _form.addIntegerInput(Message.requestPaymentDeadline());
    _productId = _form.addStringInput(Message.requestProductKey());
    _amount = _form.addIntegerInput(Message.requestAmount());
  }

  @Override
  public final void execute() throws DialogException {
    //FIXME implement command
	  _form.parse();
	  try {
		_receiver.registerSale(_clientId.value(), _deadline.value() , _productId.value(), _amount.value() );
	} catch (InsufficientStockException e) {
		// TODO Auto-generated catch block
		throw new UnavailableProductException(_productId.value(), _amount.value(), e.getEntrySpecification());
	} catch (BadEntryException e) {
		// TODO Auto-generated catch block
		throw new UnknownClientKeyException(e.getEntrySpecification());
	}
  }

}
