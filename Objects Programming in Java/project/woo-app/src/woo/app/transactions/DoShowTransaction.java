package woo.app.transactions;

import pt.tecnico.po.ui.Command;                                                                                                              import pt.tecnico.po.ui.DialogException;                                                                                                      import pt.tecnico.po.ui.Input;                                                                                                                import woo.Storefront;                                                                                                                        
//FIXME import other classes
import woo.app.exceptions.UnknownTransactionKeyException;
import woo.exceptions.BadEntryException;

/**
 * Show specific transaction.
 */
public class DoShowTransaction extends Command<Storefront> {

  //FIXME add input fields
	private Input<Integer> _transactionId;

  public DoShowTransaction(Storefront receiver) {
    super(Label.SHOW_TRANSACTION, receiver);
    //FIXME init input fields
    _transactionId = _form.addIntegerInput(Message.requestTransactionKey());
    
  }

  @Override
  public final void execute() throws DialogException {
    //FIXME implememt command
	  _form.parse();
	  try {
		  _display.popup(_receiver.showTransaction(_transactionId.value())); 
	  }catch(BadEntryException e) {
		  throw new UnknownTransactionKeyException(_transactionId.value());
	  }
  }

}
