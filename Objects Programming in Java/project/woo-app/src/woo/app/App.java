package woo.app;

import static pt.tecnico.po.ui.Dialog.IO;

import java.io.IOException;

import pt.tecnico.po.ui.Menu;
import woo.Storefront;
import woo.app.exceptions.FileOpenFailedException;
import woo.exceptions.BadEntryException;
import woo.exceptions.ImportFileException;

/** Main driver for the management application. */
public class App {

  /** @param args command line arguments. 
 * @throws IOException 
 * @throws BadEntryException 
 * @throws FileOpenFailedException 
 * @throws ImportFileException */
  public static void main(String[] args)/*throws BadEntryException , FileOpenFailedException */ {
    Storefront storefront = new Storefront();

    String datafile = System.getProperty("import");
    if (datafile != null) {
      try {
        storefront.importFile(datafile);
       }catch (ImportFileException e) {
    	   System.out.println("Nome de ficheiro not found");
    	   e.printStackTrace();
      }
    }

    Menu menu = new woo.app.main.Menu(storefront);
    menu.open();

    IO.close();
  }

}
