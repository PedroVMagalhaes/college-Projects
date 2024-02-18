package woo;
import java.io.Serializable;

public class Product implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 2454393891709852971L;
	private String _type;
	private String _idProduct;
	private String _idSupplier;
	private int _price;
	private int _stockcriticallevel;
	private int _stock = 0;

	
	public Product(String idProduct, int price, int stockcriticallev, String idSupplier) {

		_idProduct = idProduct;
		_idSupplier = idSupplier;
		_price = price;
		_stockcriticallevel = stockcriticallev;
	}



	public String get_type() {
		return _type;
	}



	public void set_type(String type) {
		_type = type;
	}



	public String get_idProduct() {
		return _idProduct;
	}




	public String get_idSupplier() {
		return _idSupplier;
	}



	public int get_price() {
		return _price;
	}
	
	public Boolean change_price(int new_price) {
		if(_price > new_price ) {
			_price = new_price;
			return true;
		}
		else{
			_price = new_price;
			return false;
		}

	}




	public int get_stockcriticallevel() {
		return _stockcriticallevel;
	}



	public void set_stockcriticallevel(int stockcriticallevel) {
		_stockcriticallevel = stockcriticallevel;
	}



	public int get_stock() {
		return _stock;
	}



	public void set_stock(int stock) {
		_stock = stock;
	}
	
	public void add_stock(int stock) {
		_stock += stock;
	}
	
	public void decrease_stock(int stock) {
		_stock -= stock;
	}

	
	@Override
	@SuppressWarnings("nls")
	public String toString() {
		// TODO Auto-generated method stub
		return _type + "|" + _idProduct + "|" + _idSupplier + "|" + _price + "|" + _stockcriticallevel +"|" + _stock;
	}
}
