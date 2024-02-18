package woo.app.clients;

import java.io.*;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;  
import woo.Storefront;       

public class DoChangeClientName extends Command<Storefront>{
	public String _output = "Insira o novo nome :";
	private static String _NAME_CHANGE = "Mudar Nome do Client";
	public Input<String> _name;

	public DoChangeClientName(Storefront storefront){
		super(_NAME_CHANGE, storefront);
		_name = _form.addStringInput(_output);

	}
	@Override
	public void execute(){
		//_form.parse();
		//_receiver.changeName(_name.value());
	}
}