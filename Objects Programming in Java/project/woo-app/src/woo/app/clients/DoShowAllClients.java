package woo.app.clients;

import pt.tecnico.po.ui.Command;                                                                          
import pt.tecnico.po.ui.DialogException;                                                                                                                                                                                                                    
import woo.Storefront;                                                                                                                        
//FIXME import other classes

/**
 * Show all clients.
 */
public class DoShowAllClients extends Command<Storefront> {

  //FIXME add input fields

  public DoShowAllClients(Storefront storefront) {
    super(Label.SHOW_ALL_CLIENTS, storefront);
                //FIXME init input fields
  }

  @Override
  public void execute() throws DialogException {
    //FIXME implement command
	/*  _form.parse();
	  _receiver.showAllClients(); */
	//  _display.popup(_receiver.showAllClients().toString());
	  _display.addLine(_receiver.showAllClients().toString());
	  _display.display();
  }
}
