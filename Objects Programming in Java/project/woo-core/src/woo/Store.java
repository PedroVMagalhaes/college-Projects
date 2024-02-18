package woo;

import java.io.*;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.regex.Pattern;


import woo.exceptions.*;

/**
 * Class Store implements a store.
 */
public class Store implements Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 202009192006L;

  // FIXME define attributes
  private int _date = 0;
  private float _assets = 0;
  private int _transactions_number = 0;
  private Map<Integer, Transactions> _transactions = new TreeMap<Integer, Transactions>();
  private Map<String, Client> _clients2 = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
  private Map<String, Supplier> _suppliers2 = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
  private Map<String, Product> _products2 = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
  // FIXME define contructor(s)
  // FIXME define methods

  /**
   * @param txtfile filename to be loaded.
   * @throws IOException
   * @throws BadEntryException
 * @throws DuplicateClientKeyException 
   */
 
  
  void importFile(String txtfile) throws ImportFileException{
    //FIXME implement method
	  
	  try {
		  BufferedReader reader = new BufferedReader (new FileReader(txtfile));
		  String line;
		  while ((line = reader.readLine()) != null) {
			  String[] info = line.split("\\|");
			  Pattern clientpattern = Pattern.compile("^(CLIENT)");
			  Pattern boxpattern = Pattern.compile("^(BOX)");
			  Pattern containerpattern = Pattern.compile("^(CONTAINER)");
			  Pattern bookpattern = Pattern.compile("^(BOOK)");
			  Pattern supplierpattern = Pattern.compile("^(SUPPLIER)"); 
			  
			  if(clientpattern.matcher(info[0]).matches()) {
				  registerClientfromimport (info[1], info[2], info[3]);			  			  
			  }
			  
			  else if(boxpattern.matcher(info[0]).matches()) {
				  registerBoxfromimport(info[1], info[2], info[3], Integer.parseInt(info[4]), Integer.parseInt(info[5]), Integer.parseInt(info[6])) ;
			  }
			  else if(containerpattern.matcher(info[0]).matches()) {
				  registerContainerfromimport(info[1], info[2], info[3], info[4], Integer.parseInt(info[5]), Integer.parseInt(info[6]), Integer.parseInt(info[7]));
			  }
			  else if(bookpattern.matcher(info[0]).matches()) {
				  registerBookfromImport(info[1], info[2], info[3], info[4], info[5], Integer.parseInt(info[6]), Integer.parseInt(info[7]), Integer.parseInt(info[8]));
			  }
			  else if(supplierpattern.matcher(info[0]).matches()) {
				  registerSupplierfromimport(info[1], info[2], info[3]);
			  }

		  }
		  
		  reader.close();
	  }
	  catch(IOException  e)  {
		  throw new ImportFileException(txtfile);
		  
	  }
	
	  
  }
  
  public String check2() {
	  double price = 0;
	  int trans = 0;
	  String fixed;
	  Sale s;
	  Order o;
	  for (int i = 0; i < _transactions.size(); i++) {
		  Transactions t = _transactions.get(i);
		  if(t.isSale()) {
			  s = (Sale) t;
			  if (price < s.get_baseprice()) {
				  price = s.get_baseprice();
				  trans = i;
			  }
		  }
		  else {
			  o = (Order) t;
			  if(price < o.get_totalcost()) {
				  price = o.get_totalcost();
				  trans = i;
			  }
		  }
			  
	  }
	  if(_transactions.get(trans).isSale()) {
		  s = (Sale) _transactions.get(trans); 
		  fixed = s.toString();
	  }
	  else {
		  o = (Order) _transactions.get(trans);
		  fixed = o.toString();
	  }
	  
	  return fixed;
		  											
  }
  
  public String check() {
	  String fixed;
	  Client c, d = null;
	  double points = 0;
	  for(Map.Entry<String, Client> entry : _clients2.entrySet()) {
		    c = entry.getValue();
		    if(points < c.get_points()) {
		    	points = c.get_points();
		    	d = c;
		    }
	  }
	  if(d != null) {
		  fixed = d.toString();
		  return fixed;
	  }
	  fixed = "no client as points";
	  
	  return fixed;
  }
	  
	  
  public void registerClient (String id, String name, String address) throws BadEntryException {
	  if (_clients2.get(id) != null)
		  throw new BadEntryException (id);
	  else {
	  	  Client c = new Client(id, name, address);
	  	  _clients2.put(c.get_id(), c);
	  }

  }
  
  public void registerClientfromimport (String id, String name, String address){

	  	  Client c = new Client(id, name, address);
	  	  _clients2.put(c.get_id(), c);

  }
  

  public String getClient (String id) throws BadEntryException{
	  String fixedstring;
	  if (_clients2.get(id) != null) {
		 Client c = _clients2.get(id);
		 fixedstring = c.toString()
				 .replace("\n, ", "\n")
				 .replace("[" , "")
				 .replace("]", "");
		 return fixedstring;
	  }
	  else { 
		  throw new BadEntryException (id); 
	  }	
		 
  }
  
  public String ShowAllClients() {
	  	String fixedstring = _clients2.values().toString()
			  .replace("\n, ", "\n")
			  .replace("[" , "")
			  .replace("]", "");
	  
	  	return fixedstring;
	  }
 
  
  
  public String showClientTransactions(String clientid) throws BadEntryException {
	  int i ;
	  String fixedstring;
	  String transactions = "";
	  Sale sale;
	  
	  Client c = _clients2.get(clientid);
	  for(i = 0; i < c.printTransactions().size(); i++) {
		  sale = (Sale) _transactions.get(c.printTransactions().get(i));
		  transactions += sale.toString(_date); 
	  }
	  fixedstring = transactions
			  .replace("\n, ", "\n")
			  .replace("[" , "")
			  .replace("]", "");
	  
	  return fixedstring;
  }
  
  
 //----------------------PRODUCTS---------------------------------// 

  
  /**
   * @param idProduct is a product id from the product being registered.
   * @param price from the product being registered
   * @param stockcriticallev critical level of stock that the product reach's
   * @param idSupplier is a supplier id from the product being registered
   * @param serviceType is the type of service that the product is assigned to
 * @throws WrongServiceTypeException 
 * @throws UnknownSupplierException 
   */ 
  public void registerBox(String idProduct, int price, int stockcriticallev, String idSupplier, String serviceType ) throws BadEntryException, WrongServiceTypeException, UnknownSupplierException {
	  if(_suppliers2.get(idSupplier) != null) {
		  if (_products2.get(idProduct) == null) {
			  if (serviceType.equals("NORMAL") || serviceType.equals("AIR") || serviceType.equals("EXPRESS") || serviceType.equals("PERSONAL")) {
				  Product product = new Box(idProduct, price, stockcriticallev,idSupplier, serviceType);
				  _products2.put(idProduct, product);
			  }
			  else {
				  throw new WrongServiceTypeException (serviceType);
			  }
		  }
		  else {
			  throw new BadEntryException (idProduct); 
			  
		  }
		  
	  } else {
		  throw new UnknownSupplierException (idSupplier); 
		  
	  }
  }
  
  /**
   * @param idProduct is a product id from the product being registered.
   * @param price from the product being registered
   * @param stockcriticallev critical level of stock that the product reach's
   * @param idSupplier is a supplier id from the product being registered
   * @param serviceType is the type of service that the product is assigned to
   * @param serviceQuality level of quality that the product offers
 * @throws WrongServiceTypeException 
 * @throws WrongServiceLevelException 
 * @throws UnknownSupplierException 
   */ 
  public void registerContainer(String idProduct, int price, int stockcriticallev, String idSupplier, String serviceType, String serviceQuality ) throws BadEntryException, WrongServiceTypeException, WrongServiceLevelException, UnknownSupplierException {
	  if(_suppliers2.get(idSupplier) != null) {
		  if (_products2.get(idProduct) == null) {
			  if(serviceQuality.equals("B4") || serviceQuality.equals("C4") || serviceQuality.equals("C5") || serviceQuality.equals("DL")) {
				  if (serviceType.equals("NORMAL") || serviceType.equals("AIR") || serviceType.equals("EXPRESS") || serviceType.equals("PERSONAL")) {
				  Product product = new Container(idProduct, price, stockcriticallev,idSupplier, serviceType, serviceQuality);
				  _products2.put(idProduct, product);
				  }
				  else {
					  throw new WrongServiceTypeException (serviceType);
				  }
			  }
			  else {
				  throw new WrongServiceLevelException (serviceQuality);
			  }
	
		  }
		  else {
			  throw new BadEntryException (idProduct); 
			  
		  }
	  } else {
		  throw new UnknownSupplierException (idSupplier);
	  }
  }
  
  /**
   * @param idProduct is a product id from the product being registered.
   * @param price from the product being registered
   * @param stockcriticallev critical level of stock that the product reach's
   * @param idSupplier is a supplier id from the product being registered
   * @param title is title of the product
   * @param author is the author of the product
   * @param isbn is the isbn of the product
 * @throws UnknownSupplierException 
   */ 
  public void registerBook(String idProduct, int price, int stockcriticallev, String idSupplier, String title,String author,String isbn ) throws BadEntryException, UnknownSupplierException{
	  if(_suppliers2.get(idSupplier) != null) {
		  if (_products2.get(idProduct) == null) {
			  Book product = new Book(idProduct, price, stockcriticallev,idSupplier, title, author, isbn );
			  _products2.put(idProduct, product);
		  }
		  else {
			  throw new BadEntryException (idProduct); 
			  
		  }
	  }
	  else {
		  throw new UnknownSupplierException (idSupplier);
	  }

  }
//book id titulo autor isbn id-fornecedor preco valor critico explarares
  
  
  public void registerBookfromImport(String idProduct, String title, String author,  String isbn, String idSupplier, int price, int stockcriticallev, int stock ) {

	  Book product = new Book(idProduct, title, author, isbn, idSupplier, price, stockcriticallev, stock );
	  _products2.put(idProduct, product);


  }
  
  public void registerContainerfromimport(String idProduct, String serviceType, String serviceQuality, String idSupplier, int price, int stockcriticallev, int stocks ) {
	  Product product = new Container( idProduct , serviceType, serviceQuality, idSupplier, price, stockcriticallev, stocks);
	  _products2.put(idProduct, product);
	  
  }
  
  
  public void registerBoxfromimport(String idProduct, String serviceType, String idSupplier, int price, int stockcriticallev, int stocks ) {
	  Product product = new Box(idProduct , serviceType, idSupplier, price, stockcriticallev, stocks);
	  _products2.put(idProduct, product);
	  
  }


  public String ShowAllProducts() {
	  	String fixedstring = _products2.values().toString()
	  		.replace("\n, ", "\n")
	  		.replace("[" , "")
	  		.replace("]", "");
	  
	  	return fixedstring;
  }
  
  public Product showProduct (String id) throws BadEntryException{
	  if (_products2.get(id) != null) {
		 Product p = _products2.get(id);
		 return p;
	  }
	  else { 
		  throw new BadEntryException (id); 
	  }	
		 
  }
  
  public String getProduct (String id) throws BadEntryException{
	  String fixedstring;
	  if (_products2.get(id) != null) {
		 Product p = _products2.get(id);
		 fixedstring = p.toString()
				 .replace("\n, ", "\n")
				 .replace("[" , "")
				 .replace("]", "");
		 return fixedstring;
	  }
	  else { 
		  throw new BadEntryException (id); 
	  }	
		 
  }
  
  public void change_price(String id , int new_price) {
	  if (_products2.get(id) != null) {
		 Product p = _products2.get(id);
		 p.change_price(new_price);
	  }
  }
  
  
//----------------Supplier-----------------------------------//
  
  
  public void registerSupplier(String id, String name, String address) throws BadEntryException{
	  if (_suppliers2.get(id) == null) {
		  Supplier s = new Supplier(id, name, address);
		  _suppliers2.put(id, s);
	  }
	  else {
		  throw new BadEntryException(id);
	  }
  }
  
  public void registerSupplierfromimport(String id, String name, String address){
		  Supplier s = new Supplier(id, name, address);
		  _suppliers2.put(id, s);
	  

  }
  
  
  
  public String ShowAllSuppliers() {
	  	String fixedstring = _suppliers2.values().toString()
	  		.replace("\n, ", "\n")
	  		.replace("[" , "")
	  		.replace("]", "");
	  
	  	return fixedstring;
	  }
  
  public Supplier showSupplier (String id) throws BadEntryException{

	  if (_suppliers2.get(id) != null) {
		 Supplier s = _suppliers2.get(id);
		 return s;
	  }
	  else { 
		  throw new BadEntryException (id); 
	  }	
		 
  }
  
  public String getSupplier (String id) throws BadEntryException{
	  String fixedstring;
	  if (_suppliers2.get(id) != null) {
		  Supplier s = _suppliers2.get(id);
		 fixedstring = s.toString()
				 .replace("\n, ", "\n")
				 .replace("[" , "")
				 .replace("]", "");
		 return fixedstring;
	  }
	  else { 
		  throw new BadEntryException (id); 
	  }	
		 
  }
  
  public Boolean toggleTransactions(String id) throws BadEntryException {
	  Supplier s = showSupplier(id);
	  Boolean toggled = s.change_activity();
	  return toggled;
	  
  }
  
  public String showSupplierTransactions(String id) throws BadEntryException {
	  	String fixedstring;
	  	Supplier s = showSupplier (id);
		if (s.get_activity().equals("NÃO")) {
			throw new BadEntryException(id);
		}
		else {
			int i ;
			 String transactions = "";
			 Order order;
			  
			  for(i = 0; i < _transactions.size(); i++) {									
				  if(_transactions.get(i).isSale()) {
					  
				  }
				  else {
					  order = (Order) _transactions.get(i);
					  transactions += order.toString();   
				  }
			  }
			  fixedstring = transactions
					  .replace("\n, ", "\n")
					  .replace("[" , "")
					  .replace("]", "");
			
		}
	  return fixedstring;
  }
  
  
 //------------------DATES-------------------------------// 
  
  public int GetDate () {

	  return _date;
  }
  
  public void AdvanceDate(int number) {
	  _date += number;
	  
  }
  
  
  //------------------Transactions------------------------//
  public void registerOrder(String supplierid, String productid, int productquantity) throws  BadEntryException{
	  
		Supplier s = showSupplier (supplierid);
		if (s.get_activity().equals("NÃO")) {
			throw new BadEntryException(supplierid);
		}
		Product p = showProduct(productid);
			Order order = new Order(_transactions_number, s, p, productquantity, _date);
			_transactions.put(_transactions_number, order );
			_assets -= order.get_totalcost();
			p.add_stock(productquantity);
			
			_transactions_number++;
		
  }
  public void registerOrderAdded(String productid, int productquantity) throws  BadEntryException{
	  Product p = showProduct(productid);
	  Order order = (Order) _transactions.get(_transactions_number-1);
	  order.addProducts(p, productquantity);
	  _assets -= order.get_totalcost();
	  p.add_stock(productquantity);
		
} 
  
  
  public void registerSale(String clientid, int saleDeadline, String productid, int productquantity) throws InsufficientStockException, BadEntryException{

		Client c = _clients2.get(clientid);
		Product p = showProduct(productid);
		if(p.get_stock() < productquantity) {
			throw new InsufficientStockException(p.get_stock());
		}
		else {
			Sale sale = new Sale(_transactions_number, _date, saleDeadline, p, productquantity, c);
			_transactions.put(_transactions_number, sale);
			c.addTransaction(_transactions_number);
			c.addSalesValue(p.get_price() * productquantity);
			p.decrease_stock(productquantity);
			
			_transactions_number++;
		}
  }
  
  public void pay (int sale_id) {
	  int _number_of_days_delayed;
	  Client c;
	  double points;
	  if (_transactions.get(sale_id).isSale() == true) {
	
		  Sale sale = (Sale) _transactions.get(sale_id);
		  if (sale.isPaid() == false) {
			  c = _clients2.get(sale.get_client().get_id());
			  if (sale.get_timeTillPenality() - _date >= sale.get_N()) {
				  sale.set_typeofP("P1");
				  sale.pay(_date);
				  c.addPoints(sale.get_finalprice() * 10);
			  }
			  if (sale.get_timeTillPenality() - _date >= 0 && sale.get_timeTillPenality() - _date < sale.get_N()) {
				  sale.set_typeofP("P2");
				  sale.pay(_date);
				  c.addPoints(sale.get_finalprice() * 10);
			  }
			  if (_date - sale.get_timeTillPenality() > 0 &&_date - sale.get_timeTillPenality() <= sale.get_N()) {
				  _number_of_days_delayed = _date - sale.get_timeTillPenality();
				  if(c.get_status().equals("ELITE") && _number_of_days_delayed > 15) {

					  sale.set_typeofP("P3");
					  sale.pay(_date);
					  c.set_status_Selection();
					  points = c.get_points() * 0.75;
					  c.subtractPoints(points);
				  }
				  if(c.get_status().equals("SELECTION") && _number_of_days_delayed > 2) {

					  sale.set_typeofP("P3");
					  sale.pay(_date);
					  c.set_status_Normal();
					  points = c.get_points() * 0.9;
					  c.subtractPoints(points);
				  }
				  else {

					  sale.set_typeofP("P3");
					  sale.pay(_date);
					  points = c.get_points() * 0.9;
					  c.subtractPoints(points);
				  }

			  }
			  if (_date - sale.get_timeTillPenality() > sale.get_N()) {
				  _number_of_days_delayed = _date - sale.get_timeTillPenality();
				  if(c.get_status().equals("ELITE") && _number_of_days_delayed > 15) {

					  sale.set_typeofP("P4");
					  sale.pay(_date);
					  c.set_status_Selection();
					  points = c.get_points() * 0.75;
					  c.subtractPoints(points);
				  }
				  if(c.get_status().equals("SELECTION") && _number_of_days_delayed > 2) {

					  sale.set_typeofP("P4");
					  sale.pay(_date);
					  c.set_status_Normal();
					  points = c.get_points() * 0.9;
					  c.subtractPoints(points);
				  }
				  else {
					  
					  sale.set_typeofP("P4");
					  sale.pay(_date);
					  points = c.get_points() * 0.9;
					  c.subtractPoints(points);
				  }
				  
			  }
			  _assets += sale.get_finalprice();
			  c.addSalesPaid(sale.get_finalprice());

			  
		  }
	  }
		
  }
  
  public String showAllTransactions () {
	  String fixedstring = _transactions.values().toString()
		  		.replace("\n, ", "\n")
		  		.replace("[" , "")
		  		.replace("]", "");
	  
	  return fixedstring;
  }
  
  public String showTransaction (int id) throws BadEntryException{
	  Transactions t = _transactions.get(id); 
	  String fixedstring;
	  if (t != null) {
		  if (t.isSale() == true) {
			  Sale sale = (Sale) t;
			  fixedstring = sale.toString(_date)
					  	.replace("\n, ", "\n")
					 	.replace("[" , "")
					 	.replace("]", "");
			  return fixedstring;
		  }
		  else{
			  fixedstring = t.toString()
					  	.replace("\n, ", "\n")
					 	.replace("[" , "")
					 	.replace("]", "");
			  return fixedstring;
		  }

	  }
	  else {
		  String error = Integer.toString(id);
		  throw new BadEntryException (error); 
	  }	
		 
  }
		 
  
  
// ---------------- Global Balance ---------------------///////
  
  
  public int showGlobalBalance () {
	  
	  return (int) _assets;
  } 
    

  
}
