package woo;
import java.io.*;
import java.util.ArrayList;
import java.util.TreeMap;
import woo.Product;


public class Transactions implements Serializable {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 175876872510307010L;
	private int _id;
	private Boolean _isSale = false;


	public Transactions(int id) {
		
		_id = id;
	}
	
	public Transactions(int id, Boolean isSale) {
		
		_id = id;
		_isSale = isSale;
	}
	
	public Boolean isSale() {
		return _isSale;
	}
	
	
	@Override
	@SuppressWarnings("nls")
	public String toString() {
		// TODO Auto-generated method stub
		return _id + "|";
	}
}
