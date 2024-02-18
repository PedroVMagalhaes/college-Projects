package woo.app.main;

 

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
//FIXME import other classes

/**
 * Show current date.
 */
public class DoDisplayDate extends Command<Storefront> {

  //FIXME add input fields
	private Input<Integer> _date;
	private int _date2;

  public DoDisplayDate(Storefront receiver) {
    super(Label.SHOW_DATE, receiver);
    //FIXME init input fields
    
    //System.out.println(_date.value()); 
    
  }

  @Override
  public final void execute() throws DialogException {
    //FIXME implement command
	  //_receiver.getDate();
	  _date2 = _receiver.getDate();
	  _display.popup(Message.currentDate(_date2));
	  // _display.popup(_date.value());
  }
}
