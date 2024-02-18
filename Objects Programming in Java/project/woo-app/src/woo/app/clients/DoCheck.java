package woo.app.clients;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Dialog;
import woo.Storefront;

public class DoCheck extends Command<Storefront> {
	
	private static String _label = "Check Highest point Client";
	
	
	public DoCheck(Storefront storefront) {
		super(_label, storefront);
	}
	
	public void execute() {
		_display.popup(_receiver.check());
	}

}
