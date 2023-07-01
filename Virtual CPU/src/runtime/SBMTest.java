package runtime;

import static org.junit.Assert.*;

import java.util.EmptyStackException;

import org.junit.Test;

public class SBMTest {

	@Test //eg2
	public void test1() {
		Runtime r = new Runtime();
		r.readFromFile("eg2.pgm"); // Load and parse mini-bytecode program
		r.run(); // execute program
		//System.out.println(r);  // print resulting state of the SBM
		assertTrue(r.emptyMem());
		assertEquals(120.00, r.getResult(), 0);
	}
	
	@Test //eg1
	public void test2() {
		Runtime r = new Runtime();
		r.readFromFile("eg1.pgm"); // Load and parse mini-bytecode program
		r.run(); // execute program
		//System.out.println(r);  // print resulting state of the SBM
		assertTrue(r.getMem()==8.4567);
	}
	
	@Test //eg5
	public void test3() {
		Runtime r = new Runtime();
		r.readFromFile("eg5.pgm"); // Load and parse mini-bytecode program
		r.run(); // execute program
		//System.out.println(r);  // print resulting state of the SBM
		assertTrue(r.getMem()==60);
	}
	
	@Test //eg3 checks for Empty Stack Exception
	public void test4() {
		Runtime r = new Runtime();
		r.readFromFile("eg3.pgm"); // Load and parse mini-bytecode program
		assertThrows(EmptyStackException.class, ()-> r.run());
	}
	
	@Test //eg4
	public void test5() {
		Runtime r = new Runtime();
		r.readFromFile("eg4.pgm"); // Load and parse mini-bytecode program
		r.run();
		assertTrue(r.getMem()==5);
	}
	
	
	
	
	

}
