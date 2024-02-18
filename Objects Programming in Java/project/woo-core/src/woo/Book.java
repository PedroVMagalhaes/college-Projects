package woo;
import woo.Product;

public class Book extends Product {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -4825395612942646490L;
	private String _title;
	private String _author;
	private String _isbn;

	public Book(String idProduct, int price, int stockcriticallev, String idSupplier, String title, String author, String isbn) {
		super(idProduct, price, stockcriticallev, idSupplier);
		_title = title;
		_author = author;
		_isbn = isbn;
		this.set_type("BOOK");
		this.set_stock(0);
	}
	//book id titulo autor isbn id-fornecedor preco valor critico explarares
	public Book(String idProduct, String title, String author,  String isbn, String idSupplier, int price, int stockcriticallev, int stock) {
		super(idProduct, price, stockcriticallev, idSupplier);
		_title = title;
		_author = author;
		_isbn = isbn;
		this.set_type("BOOK");
		this.set_stock(stock);
	}
	
	
	public String get_title() {
		return _title;
	}

	public String get_author() {
		return _author;
	}


	public String get_isbn() {
		return _isbn;
	}



	@Override
	@SuppressWarnings("nls")
	public String toString() {
		// TODO Auto-generated method stub
		return super.toString() + "|" + _title + "|" + _author + "|" + _isbn + "\n";
	}

}
