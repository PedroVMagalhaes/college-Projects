package woo;
import woo.Transactions;
import woo.Product;
import woo.Client;

public class Sale extends Transactions{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -9196756705992648648L;
	private int _saleTime;
	private int _paidTime;
	private int _N;
	private int _timeTillPenality;
	private int _numberToBuy;
	private Product _product;
	private String _productType;
	private Client _client;
	private boolean _paid = false;
	private double _baseprice;
	private double _finalprice;
	private String _typeofP;

	public Sale(int id, int currentTime, int timetopay, Product product, int productquantity, Client client) {
		super(id, true);
		_saleTime = currentTime;
		_product = product;
		_client = client;
		_timeTillPenality = timetopay;
		_numberToBuy = productquantity;
		_baseprice = _product.get_price() *  productquantity;
		
		if(_product.get_type().equals("BOOK")) {
			_N = 3;
		}
		if(_product.get_type().equals("BOX")) {
			_N = 5;
		}
		if(_product.get_type().equals("CONTAINER")) {
			_N = 8;
		}
		
	}
	
	public boolean isPaid() {
		return _paid;
	}
	
	public void pay(int date) {
		double time;
		double penality = 0;
		_paidTime = date;
		
		if(_typeofP.equals("P1") ) {
			
			_finalprice = _baseprice * 0.9;
		
		}	
		if(_typeofP.equals("P2") ) {
			if (_client.get_status().equals("NORMAL")) {
				_finalprice = _baseprice;
			}
			if (_client.get_status().equals("SELECTION")) {
				if (_timeTillPenality - date >= 2) {
					_finalprice = _baseprice * 0.95;
				}
				else {
					_finalprice = _baseprice;
				}
			}
			if (_client.get_status().equals("ELITE")) {
				_finalprice = _baseprice * 0.9;
			}
			
		}
		if(_typeofP.equals("P3") ) {
			if (_client.get_status().equals("NORMAL")) {
				time =date - _timeTillPenality;
				
				for(int i = 0; i < time; i++) {
					penality += 0.05;
				}
				
				_finalprice = _baseprice * (1 + penality);
			}
			if (_client.get_status().equals("SELECTION")) {
				if (date - _timeTillPenality <= 1) {
					_finalprice = _baseprice;
				}
				time =date - _timeTillPenality;
				
				for(int i = 0; i < time; i++) {
					penality += 0.02;
				}
				
				_finalprice = _baseprice * (1 + penality);
			}
			if (_client.get_status().equals("ELITE")) {
				_finalprice = _baseprice * 0.95;
			}
		}
		if(_typeofP.equals("P4") ) {
			if (_client.get_status().equals("NORMAL")) {
				time =date - _timeTillPenality;
				
				for(int i = 0; i < time; i++) {
					penality += 0.10;
				}
				
				_finalprice = _baseprice * (1 + penality);
			}
			if (_client.get_status().equals("SELECTION")) {
				time =date - _timeTillPenality;
				
				for(int i = 0; i < time; i++) {
					penality += 0.05;
				}
				
				_finalprice = _baseprice * (1 + penality);
			}
			if (_client.get_status().equals("ELITE")) {
				_finalprice = _baseprice;
			}
		}
		
		
		_paid = true;
	}
	
	public void checkingpaidedprice(int date) {
		double time;
		double penality = 0;
		_paidTime = date;
		
		
		if(_typeofP.equals("P1") ) {
			
			_finalprice = _baseprice * 0.9;

		
		}	
		if(_typeofP.equals("P2") ) {
			
			
			if (_client.get_status().equals("NORMAL")) {
				_finalprice = _baseprice;
			}
			if (_client.get_status().equals("SELECTION")) {
				if (_timeTillPenality - date >= 2) {
					_finalprice = _baseprice * 0.95;
				}
				else {
					_finalprice = _baseprice;
				}
			}
			if (_client.get_status().equals("ELITE")) {
				_finalprice = _baseprice * 0.9;
			}
			
		}
		if(_typeofP.equals("P3") ) {
			
			
			if (_client.get_status().equals("NORMAL")) {
				time =date - _timeTillPenality;
				
				for(int i = 0; i < time; i++) {
					penality += 0.05;
				}
				
				_finalprice = _baseprice * (1 + penality);
			}
			if (_client.get_status().equals("SELECTION")) {
				if (date - _timeTillPenality <= 1) {
					_finalprice = _baseprice;
				}
				time =date - _timeTillPenality;
				
				for(int i = 0; i < time; i++) {
					penality += 0.02;
				}
				
				_finalprice = _baseprice * (1 + penality);
			}
			if (_client.get_status().equals("ELITE")) {
				_finalprice = _baseprice * 0.95;
			}
		}
		if(_typeofP.equals("P4") ) {
			
			
			if (_client.get_status().equals("NORMAL")) {
				time =date - _timeTillPenality;
				
				for(int i = 0; i < time; i++) {
					penality += 0.10;
				}
				
				_finalprice = _baseprice * (1 + penality);
			}
			if (_client.get_status().equals("SELECTION")) {
				time =date - _timeTillPenality;
				
				for(int i = 0; i < time; i++) {
					penality += 0.05;
				}
				
				_finalprice = _baseprice * (1 + penality);
			}
			if (_client.get_status().equals("ELITE")) {
				_finalprice = _baseprice;
			}
		}

	}
	
	public int get_paidtime() {
		return _paidTime;
	}

	public int get_numberToBuy() {
		return _numberToBuy;
	}
	
	public double get_baseprice() {
		return _baseprice;
	}
	
	public int get_N() {
		return _N;
	}

	public String get_typeofP() {
		return _typeofP;
	}

	public void set_typeofP(String typeofP) {
		_typeofP = typeofP;
	}

	public int get_saleTime() {
		return _saleTime;
	}

	public int get_paidTime() {
		return _paidTime;
	}

	public int get_timeTillPenality() {
		return _timeTillPenality;
	}

	public Product get_product() {
		return _product;
	}

	public String get_productType() {
		return _productType;
	}

	public Client get_client() {
		return _client;
	}

	public double get_finalprice() {
		return _finalprice;
	}
	

	@SuppressWarnings("nls")
	public String toString(int date) {
		// TODO Auto-generated method stub
		int intfinalprice;
		int intbaseprice = (int) Math.round(_baseprice);
		if(_paid == true) {
			intfinalprice = (int) Math.round(_finalprice);
			return super.toString() + _client.get_id() + "|" + _product.get_idProduct() + "|" + _numberToBuy + "|" + intbaseprice  + "|" + intfinalprice + "|" + _timeTillPenality + "|" + _paidTime + "\n";
		}
		else {
				if (_timeTillPenality - date >= _N) {
					_typeofP = "P1";
				}
				if (_timeTillPenality - date >= 0 && _timeTillPenality - date < _N) {
					_typeofP = "P2";
				}
				if (date - _timeTillPenality > 0 && date - _timeTillPenality <= _N) {
					_typeofP = "P3";
				}
				if (date - _timeTillPenality > _N ) {
					_typeofP = "P4";
				}
				checkingpaidedprice(date);
				intfinalprice = (int) Math.round(_finalprice);
				return super.toString() + _client.get_id() + "|" + _product.get_idProduct() + "|" + _numberToBuy + "|" + intbaseprice  + "|" + intfinalprice + "|" + _timeTillPenality + "\n";
			}
		}


}
	
	
	
	


