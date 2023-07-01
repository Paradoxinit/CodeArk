package runtime;
/**
 * @author Raqeebullah Hasan
 */
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Stack;


public class Runtime {
	// data fields
	private ArrayList<Instruction> pgm;
	private int pc;
	private Stack<Double> stack;
	private ArrayList<Double> memory;
	
	public double getResult() {
		return stack.peek();
	}
	public double getMem() {
		return memory.get(0);
	}
	public boolean emptyMem(){
		for(double x:memory) {
			if(x!=0)
				return false;
		}
		return true;
	}
	Runtime() {
		stack = new Stack<>();
		memory = new ArrayList<>(10);	
	}
	
	/**	 * Set SBM to initial state
	 */
	private void initialize() {
		// empty stack
		while (!stack.isEmpty()) {
			stack.pop();
		}
		// reset all memory locations
		for (int i=0; i<10; i++) {
			memory.add(0.0);
		}
		// set pc to first instruction
		pc=1;
	}
	/**
	 * return the code line where the labelName is declared
	 * @param label is the name of the declared label.
	 * @return the step where the label is first declared.
	 */
     private int jumpToLabel(String label) {// Implement
    	 int i = 0;
    	 boolean found = false;
    	 while ( i < pgm.size() && !found) {
    		 Instruction inst = pgm.get(i);
    		 found = inst.code == 8 && ((Label) inst).getName().equals(label);
    		 i++;
    	 }
    	 if(!found) throw new IllegalArgumentException("runtime: label not found!");
    	 return i;
	}
	/**
	 * Given an instruction, this method accordingly 
	 * updates the memory, stack and program counter.
	 * @param i
	 * @return
	 */
	private boolean processInstruction(Instruction i) {
	    // Implement
		switch(i.code) {
		case 0:
			pc++;
			return false;
		case 1:
			stack.push( ((PushLiteral) i).getLiteral());
			pc++;
			break;
		case 2:
			stack.push(memory.get( ((PushLocation)i).getAddress()) );
			pc++;
			break;
		case 3:
			int index = ((Pop) i).getAddress();
			memory.set(index, stack.pop());
			pc++;
			break;
		case 4:
			stack.push(stack.pop()+stack.pop());
			pc++;
			break;
		case 5:
			stack.push(stack.pop()-stack.pop());
			pc++;
			break;
		case 6:
			stack.push(stack.pop()*stack.pop());
			pc++;
			break;
		case 7:
			stack.push(stack.pop()/stack.pop());
			pc++;
			break;
		case 8:
			pc++;
			break;
		case 9:
			if(stack.peek()==0)
				pc = jumpToLabel(((Jmpz) i).getTargetLabel());
			else
				pc++;
			break;
		case 10:
			pc = jumpToLabel(((Jmp) i).getTargetLabel());
			break;
		case 11:
			stack.push(stack.pop()-1); //try pop!
			pc++;
			break;
		default:
			throw new IllegalArgumentException("Invalid Instruction!");
		}
		return true;
	}
	
	/**
	 * Loops through the program array (pgm)
	 */
	public void run() { // Implement
		initialize();
		while(processInstruction(pgm.get(pc-1)));
	}
	
	private Instruction parseInstruction(String str, int line) {
		String[] p = str.split("[ ]+"); // delimiters are non-empty sequences of spaces
		Instruction i=null;

		switch (p[0]) {
		case "exit":
			i = new Exit(0,"exit");
			break;
		case "push": 
			if (p.length==1) {
				throw new IllegalStateException("parseInstruction: syntax error at line "+line);
			}
			try {
			if (p[1].charAt(0)=='m') {
				int loc = Integer.parseInt(p[1].substring(1));
				if (loc<0 || loc>9) {
					throw new IllegalStateException("parseInstruction: syntax error at line "+line);
				}
				i = new PushLocation(2,"push",loc);
			} else {
				i = new PushLiteral(1,"push",Double.parseDouble(p[1]));
			}
			} catch (NumberFormatException e) {
				throw new IllegalStateException("parseInstruction: syntax error at line "+line);
			}
			break;
		case "pop": 
			if (p.length==1) {
				throw new IllegalStateException("parseInstruction: syntax error at line "+line);
			}
			int loc = Integer.parseInt(p[1].substring(1));
			if (loc<0 || loc>9) {
				throw new IllegalStateException("parseInstruction: syntax error at line "+line);
			}
			i = new Pop(3,"pop",loc);
			break;
		case "add": 
			i = new Add(4,"add");
			break;
		case "sub": 
			i = new Sub(5,"sub");
			break;
		case "mul": 
			i = new Mul(6,"mul");
			break;
		case "div": 
			i = new Div(7,"div");
			break;
		case "label":
			i = new Label(8,"label",p[1]);
			break;
		case "jmpz":
			i = new Jmpz(9,"jmpz",p[1]);
			break;
		case "jmp":
			i = new Jmp(10,"jmp",p[1]);
			break;
		case "dec":
			i = new Dec(11,"dec");
			break;
		case "":
			break;
		default:
			throw new IllegalStateException("parseInstruction: syntax error at line "+line);
		}
		return i;
	}
	
	public void readFromFile(String name)  {
		pgm = new ArrayList<>();
		File f = new File(name);
		try {
			Scanner s = new Scanner(f);
			int line=1;
			
			while (s.hasNext()) {
				Instruction i = parseInstruction(s.nextLine(),line);
				if (i!=null) {
					pgm.add(i);
				}
				line++;
			}
			s.close();
			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	@Override
	public String toString() {
		StringBuilder r = new StringBuilder();
		r.append("Pgm   : ");
		r.append(pgm==null?"null":pgm.toString());
		r.append("\nPc    : "+pc);
		r.append("\nStack : ");
		r.append(stack.toString());
		r.append("\nMemory: ");
		r.append(memory.toString());
		r.append("\n------------------------------------------------\n");
		
		return r.toString();
	}
	
	public static void main(String[] args) {
		Runtime r = new Runtime();
		r.readFromFile("eg2.pgm"); // Load and parse mini-bytecode program
		r.run(); // execute program
		System.out.println(r);  // print resulting state of the SBM
	}
}
