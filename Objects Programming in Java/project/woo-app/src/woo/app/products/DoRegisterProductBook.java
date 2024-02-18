package woo.app.products;

import pt.tecnico.po.ui.Command;                                                                                                              
import pt.tecnico.po.ui.DialogException;                                                                                                      
import pt.tecnico.po.ui.Input;                                                                                                                
import woo.Storefront;
import woo.app.exceptions.DuplicateProductKeyException;
import woo.app.exceptions.UnknownSupplierKeyException;
//FIXME import other classes
import woo.app.exceptions.UnknownTransactionKeyException;
import woo.exceptions.BadEntryException;
import woo.exceptions.UnknownSupplierException;

/**
 * Register book.
 */
public class DoRegisterProductBook extends Command<Storefront> {

  //FIXME add input fields
	private Input<String> _productId;
	private Input<String> _title;
	private Input<String> _author;
	private Input<Integer> _price;
	private Input<Integer> _criticalstock;
	private Input<String> _supplierid;
	private Input<String> _isbn;
	
	

  public DoRegisterProductBook(Storefront receiver) {
    super(Label.REGISTER_BOOK, receiver);
    //FIXME init input fields
    _productId = _form.addStringInput(Message.requestProductKey());
    _title = _form.addStringInput(Message.requestBookTitle());
    _author = _form.addStringInput(Message.requestBookAuthor());
    _isbn = _form.addStringInput(Message.requestISBN());
    _price = _form.addIntegerInput(Message.requestPrice());
    _criticalstock = _form.addIntegerInput(Message.requestStockCriticalValue());
    _supplierid = _form.addStringInput(Message.requestSupplierKey());
    
  }

  @Override
  public final void execute() throws DialogException {
    //FIXME implement command
	  _form.parse();
	  try { 
	 _receiver.registerBook(_productId.value(), _price.value(), _criticalstock.value(), _supplierid.value(), _title.value(), _author.value(), _isbn.value());
	  }catch(BadEntryException e) {
		  throw new DuplicateProductKeyException( _productId.value());
	  }catch(UnknownSupplierException e) {
		  throw new UnknownSupplierKeyException( _supplierid.value());
	  }
  }			
}
