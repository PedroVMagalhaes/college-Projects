package woo;

import java.io.*;
import java.util.Collection;

import woo.exceptions.*;


/**
 * Storefront : façade for the core classes.
 */
public class Storefront {

  /** Current filename. */
  private String _filename = null;

  /** The actual store. */
  private Store _store = new Store();
  
  private boolean save = false;

  //FIXME define other attributes
  //FIXME define constructor(s)
  //FIXME define other methods

  /**
   * @throws IOException
   * @throws FileNotFoundException
   * @throws MissingFileAssociationException
   */
  public void save() throws IOException , FileNotFoundException, MissingFileAssociationException {
    //FIXME implement serialization method

    if (save == true){
    	
	    if (_filename == null) {
	    	ObjectOutputStream oos = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream("savefile")));
	        oos.writeObject(_store);
	        oos.close();
	        save = false;
	    }
	    else{
	    	ObjectOutputStream oos = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream(_filename)));
	        oos.writeObject(_store);
	        oos.close();
	        save = false;
    }
    }
    //System.out.println("fiz output stream");
  }

  /**
   * @param filename
   * @throws MissingFileAssociationException
   * @throws IOException
   * @throws FileNotFoundException
   */
  public void saveAs(String filename) throws MissingFileAssociationException, FileNotFoundException, IOException {
    _filename = filename;
    save();
  }

  /**
   * @param filename
   * @throws UnavailableFileException
   */
  public void load(String filename) throws FileNotFoundException, IOException, ClassNotFoundException/*, UnavailableFileException */{
    //FIXME implement serialization method
    ObjectInputStream ois = new ObjectInputStream(new BufferedInputStream(new FileInputStream(filename)));
    _store = (Store) ois.readObject();
    ois.close(); 
    _filename = filename;
  }

  /**
   * @param textfile
 * @throws IOException 
   * @throws ImportFileException
 * @throws DuplicateClientKeyException 
   */
  public void importFile(String textfile) throws ImportFileException/*, BadEntryException */ {
	  
      _store.importFile(textfile);
      save = true;

      
  }
  
  public void registerClient (String id, String name, String address) throws BadEntryException {
    _store.registerClient(id, name, address);
    save = true;
  }

  public String showClient (String id) throws BadEntryException {
	  return _store.getClient(id);
  }
  
  public String showClientTransaction (String id) throws BadEntryException {
	  return _store.showClientTransactions(id);
  }

  public String showAllClients() {
	  save = true;
	  return _store.ShowAllClients();
  }
  
  //-----------------Products ----------------------//
  
  public void registerBook(String idProduct, int price, int stockcriticallev, String idSupplier,String title,String author,String isbn) throws BadEntryException, UnknownSupplierException {
	  _store.registerBook(idProduct, price, stockcriticallev, idSupplier, title, author, isbn);
	  save = true;
  }
  public void registerContainer(String idProduct, int price, int stockcriticallev, String idSupplier,String serviceType ,String serviceQuality ) throws BadEntryException, WrongServiceTypeException, WrongServiceLevelException, UnknownSupplierException {
	  _store.registerContainer(idProduct, price, stockcriticallev, idSupplier, serviceType, serviceQuality);
	  save = true;
  }
  public void registerBox(String idProduct, int price, int stockcriticallev, String idSupplier,String serviceType) throws BadEntryException, WrongServiceTypeException, UnknownSupplierException {
	  _store.registerBox(idProduct, price, stockcriticallev, idSupplier, serviceType);
	  save = true;
  }
  
  public void changePrice(String productid , int new_price) {
	  _store.change_price(productid, new_price);
	  save =true;
  }
 
  
  public String showAllProducts() {
	  save = true;
	  return _store.ShowAllProducts();
  }

  public String showProduct (String id) throws BadEntryException {
	  save = true;
	  return _store.getProduct(id);
  }
  
  
  
  
//-------------------Suppliers----------------------------------//
  
 public String showAllSuppliers () {
	 save = true;
	 return _store.ShowAllSuppliers();
 }
 
 public void registerSupplier (String id, String name, String address) throws BadEntryException {
	 save = true;
	 _store.registerSupplier(id, name, address);
 }
 
 public String showSupplier (String id) throws BadEntryException {
	  save = true;
	  return _store.getSupplier(id);
 }
 
 public Boolean toggleTransactions(String id) throws BadEntryException {
	 return _store.toggleTransactions(id);
 }
 
 public String showSupplierTransactions(String id) throws BadEntryException{
	 return _store.showSupplierTransactions(id);
 }
  
//---------------Transactions--------------------------------------//
 
 
 public void registerSale(String clientid, int saleDeadline, String productid, int productquantity) throws BadEntryException, InsufficientStockException {
	 save = true;
	 _store.registerSale(clientid, saleDeadline, productid, productquantity);
 }
 
 public void registerOrder(String supplierid, String productid, int productquantity) throws BadEntryException {
	 save = true;
	 _store.registerOrder(supplierid, productid, productquantity);
 }
 
 public void registerOrderAdded(String productid, int productquantity) throws BadEntryException {
	 save = true;
	 _store.registerOrderAdded(productid, productquantity);
 } 
 
 public void showAllTransactions() {
	 save = true;
	 _store.showAllTransactions();
 }
 
 public String showTransaction(int id) throws BadEntryException {
	save = true;
	return _store.showTransaction(id);
 }
 
 public void pay(int id) {
	 save = true;
	 _store.pay(id);
 }
  

  
 //---------------------- Extra ---------------------------------// 
  
  
  
  
  
  public String getfilename() {
    return _filename;
  }
    
  public void setfilename(String name) {
    _filename = name;
  }
    
  public boolean getSave() {
    return save;
  }
  
  public int getDate() {
  return _store.GetDate();
  }
  
  
  public void advanceDate(int number) {
    _store.AdvanceDate(number);
    save = true;
  }
  
  public int showGlobalBalance() {
	  return _store.showGlobalBalance();
  } 
  
  public String check() {
	return _store.check();	
  }
  
} 



