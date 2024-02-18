package woo.app.products;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;
import woo.app.exceptions.DuplicateProductKeyException;
import woo.app.exceptions.UnknownServiceLevelException;
import woo.app.exceptions.UnknownServiceTypeException;
//FIXME import other classes
import woo.app.exceptions.UnknownSupplierKeyException;
import woo.exceptions.BadEntryException;
import woo.exceptions.UnknownSupplierException;
import woo.exceptions.WrongServiceLevelException;
import woo.exceptions.WrongServiceTypeException;

/**
 * Register container.
 */
public class DoRegisterProductContainer extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _productId;
	private Input<Integer> _price;
	private Input<Integer> _criticalstock;
	private Input<String> _supplierid;
	private Input<String> _transportType;
	private Input<String> _serviceLevel;

  public DoRegisterProductContainer(Storefront receiver) {
    super(Label.REGISTER_CONTAINER, receiver);
    //FIXME init input fields
    _productId = _form.addStringInput(Message.requestProductKey());
   	_price = _form.addIntegerInput(Message.requestPrice());
   	_criticalstock = _form.addIntegerInput(Message.requestStockCriticalValue());
   	_supplierid = _form.addStringInput(Message.requestSupplierKey());
   	_transportType = _form.addStringInput(Message.requestServiceType());
   	_serviceLevel = _form.addStringInput(Message.requestServiceLevel());
  }

  @Override
  public final void execute() throws DialogException {
    //FIXME implement command
	  _form.parse();
	  try {
	  _receiver.registerContainer(_productId.value(), _price.value(), _criticalstock.value(), _supplierid.value(), _transportType.value(), _serviceLevel.value());
	  }catch(BadEntryException e) {
		  throw new DuplicateProductKeyException( _productId.value());
	  }catch(WrongServiceLevelException e) {
		  throw new UnknownServiceLevelException( _serviceLevel.value());
	  }catch(WrongServiceTypeException e) {
		  throw new UnknownServiceTypeException( _transportType.value());
	  }catch(UnknownSupplierException e) {
		  throw new UnknownSupplierKeyException( _supplierid.value());
	  }
	  
  }
}
