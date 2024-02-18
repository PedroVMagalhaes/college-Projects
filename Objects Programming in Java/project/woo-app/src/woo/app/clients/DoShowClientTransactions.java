package woo.app.clients;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
//FIXME import other classes
import woo.app.exceptions.UnknownClientKeyException;
import woo.exceptions.BadEntryException;

/**
 * Show all transactions for a specific client.
 */
public class DoShowClientTransactions extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _clientId;
  public DoShowClientTransactions(Storefront storefront) {
    super(Label.SHOW_CLIENT_TRANSACTIONS, storefront);
    //FIXME init input fields
    _clientId = _form.addStringInput(Message.requestClientKey());
  }

  @Override
  public void execute() throws DialogException {
    //FIXME implement command
	  _form.parse();
	  try {
		  _display.popup(_receiver.showClientTransaction(_clientId.value()).toString());
	  }
	  catch(BadEntryException e) {
		  throw new UnknownClientKeyException(_clientId.value());
	  }
  }

}
