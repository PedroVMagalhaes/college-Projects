package woo.app.main;

import pt.tecnico.po.ui.Command;                                                                                                             
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;                                                                                                                        
//FIXME import other classes
import woo.app.exceptions.InvalidDateException;

/**
 * Advance current date.
 */
public class DoAdvanceDate extends Command<Storefront> {
  
  //FIXME add input fields
	private Input<Integer> _dateadvance;
	

  public DoAdvanceDate(Storefront receiver) {
    super(Label.ADVANCE_DATE, receiver);
    //FIXME init input fields
    _dateadvance = _form.addIntegerInput(Message.requestDaysToAdvance());
    
  }

  @Override
  public final void execute() throws DialogException {
    //FIXME implement command
	  _form.parse();
	  	if(_dateadvance.value() < 0) {
	  		throw new InvalidDateException(_dateadvance.value());
	  	}
		  _receiver.advanceDate(_dateadvance.value());

  }
}
