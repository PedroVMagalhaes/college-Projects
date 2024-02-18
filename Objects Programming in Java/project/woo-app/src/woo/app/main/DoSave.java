package woo.app.main;

import java.io.IOException;

import pt.tecnico.po.ui.Command;                                                                                                              import pt.tecnico.po.ui.DialogException;                                                                                                      import pt.tecnico.po.ui.Input;                                                                                                                import woo.Storefront;                                                                                                                        //FIXME import other classes
import woo.exceptions.MissingFileAssociationException;

/**
 * Save current state to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _filename;

  /** @param receiver */
  public DoSave(Storefront receiver) {
    super(Label.SAVE, receiver);
    //FIXME init input fields
    if (_receiver.getfilename() == null){
    	_filename = _form.addStringInput(Message.newSaveAs());
    }
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    //FIXME implement command
	  if (_receiver.getSave()) {
		  try {
			  if(_receiver.getfilename() == null) {
				  _form.parse();
	              _receiver.setfilename(_filename.value());
	              _receiver.saveAs(_filename.value());
			  }
			  _receiver.saveAs(_receiver.getfilename());
		  }
		  catch(IOException  | MissingFileAssociationException e) {
			  e.printStackTrace();
		  } 
		}
	  }
  }

