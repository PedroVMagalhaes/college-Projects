package woo;
import java.util.ArrayList;
import java.util.TreeMap;
import woo.Product;
import woo.Supplier;
import woo.Transactions;

public class Order extends Transactions{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -1107904972882908753L;
	private double _totalcost = 0;
	//private TreeMap<String, Product> _products = new TreeMap<String, Product>();
	private TreeMap<Integer, Product> _products = new TreeMap<Integer, Product>();
	private TreeMap<String, Integer> _numberofproducts = new TreeMap<String, Integer>();
	private Supplier _supplier;
	private int _paymentDate;
	
	public Order(int id, Supplier supplier, Product product, int numberoforders, int date) {
		super(id);
		_products.put(_products.size(), product);
		_numberofproducts.put(product.get_idProduct(), numberoforders);
		_totalcost += (product.get_price() * numberoforders) ;
		_supplier = supplier;
		_paymentDate = date;
		
		
	}
	
	public void addProducts(Product product, int numberoforders) {
		_products.put(_products.size(), product);
		_numberofproducts.put(product.get_idProduct(), numberoforders);
		_totalcost += (product.get_price() * numberoforders) ;
	}

	public double get_totalcost() {
		return _totalcost;
	}
	
	@Override
	@SuppressWarnings("nls")
	public String toString() {
		// TODO Auto-generated method stub
		int intfinalprice = (int) Math.round(_totalcost);
		String ret = super.toString() + _supplier.get_id() + "|" + intfinalprice + "|" + _paymentDate +"\n";
		for (int i = 0 ; i < _products.size() ; i++){
			Product product = _products.get(i);
			if(_products.size() - i == 1) {
				ret += product.get_idProduct() + "|" + _numberofproducts.get(product.get_idProduct()) + "\n";
			}
			else {
				ret += product.get_idProduct() + "|" + _numberofproducts.get(product.get_idProduct()) + "\n";
			}

		}
		
		return ret;
	}
	
	

}
