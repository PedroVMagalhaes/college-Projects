package woo.app.clients;

import java.io.*;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;  
import woo.Storefront;         
//FIXME import other classes
import woo.app.exceptions.DuplicateClientKeyException;
import woo.exceptions.BadEntryException;

/**
 * Register new client.
 */
public class DoRegisterClient extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _clientId;
	private Input<String> _name;
	private Input<String> _address;

  public DoRegisterClient(Storefront storefront) {
    super(Label.REGISTER_CLIENT, storefront);
    //FIXME init input fields
    _clientId = _form.addStringInput(Message.requestClientKey());
    _name = _form.addStringInput(Message.requestClientName());
    _address = _form.addStringInput(Message.requestClientAddress());
  
  }

  @Override
  public void execute() throws DialogException{
    //FIXME implement command
	  _form.parse();
	  try {
		   _receiver.registerClient(_clientId.value(), _name.value() , _address.value());
		  
	  }
	  catch(BadEntryException e) {
		  throw new DuplicateClientKeyException(_clientId.value());
	  }
/*	  catch(DuplicateClientKeyException e) {
		  throw new DuplicateClientKeyException(_name.value());
	  } */
  }

}
