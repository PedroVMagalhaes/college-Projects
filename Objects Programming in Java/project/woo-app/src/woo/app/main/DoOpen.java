package woo.app.main;

import java.io.FileNotFoundException;
import java.io.IOException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import woo.Storefront;
import woo.app.exceptions.FileOpenFailedException;
//FIXME import other classes

/**
 * Open existing saved state.
 */
public class DoOpen extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _filename;
	
  /** @param receiver */
  public DoOpen(Storefront receiver) {
    super(Label.OPEN, receiver);
    //FIXME init input fields
    _filename = _form.addStringInput(Message.openFile());
  }

  /** @throws FileOpenFailedException 
 * @throws IOException 
 * @throws ClassNotFoundException 
 * @throws FileNotFoundException 
 * @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException{
    try {
      //FIXME implement command
    	_form.parse();
        _receiver.load(_filename.value());
    /*    
    } catch (UnavailableFileException ufe) {
    	throw new FileOpenFailedException(ufe.getFilename());
    */	
    } catch (FileNotFoundException e) {
    	throw new FileOpenFailedException(_filename.value());
	
	} catch (ClassNotFoundException e) {
		e.printStackTrace();
	} catch (IOException e) {
		e.printStackTrace();
	}
    
  }

}
