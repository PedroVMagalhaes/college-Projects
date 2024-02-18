package woo;
import woo.Product;

public class Box extends Product {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 8523939669134105246L;
	private String _serviceType;

	public Box(String idProduct, int price, int stockcriticallev, String idSupplier, String serviceType) {
		super(idProduct, price, stockcriticallev, idSupplier);
		this.set_serviceType(serviceType);
		this.set_type("BOX");
		this.set_stock(0);
	}
	
	public Box(String idProduct, String serviceType, String idSupplier, int price, int stockcriticallev, int stocks) {
		super(idProduct, price, stockcriticallev, idSupplier);
		this.set_serviceType(serviceType);
		this.set_type("BOX");
		this.set_stock(stocks);
	}

	public String get_serviceType() {
		return _serviceType;
	}

	public void set_serviceType(String serviceType) {
		_serviceType = serviceType;
	}
	
	@Override
	@SuppressWarnings("nls")
	public String toString() {
		// TODO Auto-generated method stub
		return super.toString() + "|" + _serviceType + "\n";
	}

}
