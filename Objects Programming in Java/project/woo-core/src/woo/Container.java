package woo;
import woo.Product;

public class Container extends Product {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -5603557892051075241L;
	private String _serviceType;
	private String _serviceQuality;

	public Container(String idProduct, int price, int stockcriticallev, String idSupplier, String serviceType, String serviceQuality) {
		super(idProduct, price, stockcriticallev, idSupplier);
		_serviceType = serviceType;
		_serviceQuality = serviceQuality;
		this.set_type("CONTAINER");		
		this.set_stock(0);
	}
	
	public Container(String idProduct, String serviceType, String serviceQuality, String idSupplier, int price, int stockcriticallev, int stocks) {
		super(idProduct, price, stockcriticallev, idSupplier);
		_serviceType = serviceType;
		_serviceQuality = serviceQuality;
		this.set_type("CONTAINER");		
		this.set_stock(stocks);
	}

	public String get_serviceType() {
		return _serviceType;
	}

	public void set_serviceType(String serviceType) {
		_serviceType = serviceType;
	}
	

	public String get_serviceQuality() {
		return _serviceQuality;
	}

	public void set_serviceQuality(String serviceQuality) {
		_serviceQuality = serviceQuality;
	}
	
	@Override
	@SuppressWarnings("nls")
	public String toString() {
		// TODO Auto-generated method stub
		return super.toString() + "|" + _serviceType + "|" + _serviceQuality + "\n";
	}

}
