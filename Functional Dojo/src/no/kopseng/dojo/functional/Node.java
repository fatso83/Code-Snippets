package no.kopseng.dojo.functional;

import java.util.ArrayList;
import java.util.List;

/**
 * @author: Carl-Erik Kopseng
 * Date: 28.04.11
 * Purpose: Trying to maximally remove state from a standard implementation of a tree printing routine
 */
public class Node {
	private String content;
	private List<Node> children;


	public static void main(String arguments[]) {
		/* Building node hierarchy */
		Node root = new Node("root");
		Node child1 = new Node("child1");
		Node child2 = new Node("child2");
		root.add(child1);
		root.add(child2);

		Node grandchild2_1 = new Node("grandchild2_1");
		Node grandchild2_2 = new Node("grandchild2_2");
		child2.add(grandchild2_1);
		child2.add(grandchild2_2);

		Node babychild2_1_1 = new Node("babychild2_1_1");
		grandchild2_1.add(babychild2_1_1);

		/* Print nodes */
		System.out.println(root.toString());
	}

	public Node(String content) {
		this.content = content;
		this.children = new ArrayList<Node>();
	}

	public void add(Node child) {
		children.add(child);
	}


	public String toString() {
		/*
		return tabbedString1("");
		return tabbedString2("");
		*/
		return tabbedString3("");
	}

	public String tabbedString1(int level) {
		StringBuilder sb = new StringBuilder();

		for (int i = 0; i < level; i++) {
			sb.append("\t");
		}

		sb.append(content);
		sb.append("\n");

		for (Node c : children) {
			sb.append(c.tabbedString1(level + 1));
		}

		return sb.toString();
	}

	public String tabbedString2(String indentation) {
		String childTree = "";
		for (Node child : children) {
			childTree += child.tabbedString2(indentation + "\t");
		}

		return indentation + content + "\n" + childTree;
	}

	public String tabbedString3(final String indentation) {

		return indentation + content + "\n" +
				concat(
						map(children, new Function() {
							public Object exec(Object child) {
								return ((Node) child).tabbedString3(indentation + "\t");
							}
						})
				);
	}

	private List map(List elems, Function function) {
		List returnList = new ArrayList();

		for (Object elem : elems) {
			returnList.add(function.exec(elem));
		}

		return returnList;
	}

	private String concat(List<String> strings) {
		StringBuilder sb = new StringBuilder();

		for (String s : strings)
			sb.append(s);
		return sb.toString();
	}


	/* Husker ikke hvordan vi skulle gj?re dette vha Generics ... */
	interface Function {
		public Object exec(Object o);
	}

	interface ReduceFunction {
		public Object exec(List o);
	}
}
