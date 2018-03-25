package mykorat;
import java.util.HashSet;
import java.util.LinkedList;

import korat.finitization.IFinitization;
import korat.finitization.IObjSet;
import korat.finitization.IIntSet;
import korat.finitization.IArraySet;
import korat.finitization.impl.FinitizationFactory;

public class AcyclicGraph {

    public static class Node {  
        Node[] AdjList;  // save the nodes that the current node points to
    }
    
    private Node[] AllNodes;  // save all the nodes in the graph

    private int size;

    public boolean repOK() {
    	Node CurrentNode,NextNode;
    	LinkedList<Node> WorkList=new LinkedList<Node>();
    	HashSet<Node> VisitedSet=new HashSet<Node>(); 
    	
        if(AllNodes.length==0) 
        	return size==0;
        
        /* check if the graph has circles:
         * start from each node in the graph and move to the nodes it points to
         * if exist a way come back to the initial point then return false
         */
        
        for(int i=0;i<AllNodes.length;i++) {
        	/* For every iteration, create a new WorkList and a new VisitedList
        	 * WorkList serves as a queue to save all nodes that can be arrived
        	 * VisitedList saves the visited nodes to prevent endless loop 
        	 */
     
        	WorkList.clear();
        	VisitedSet.clear();
        	WorkList.add(AllNodes[i]);
        	
        	while(!WorkList.isEmpty()) {
        		CurrentNode=WorkList.removeFirst(); 
        		for(int j=0;j<CurrentNode.AdjList.length;j++) {
        				NextNode=CurrentNode.AdjList[j];
        				
        				// if return to the original node, return false
        				if(AllNodes[i]==NextNode)  
        					return false;
        				
        				// if not visited, save in the WorkList
        				else if(VisitedSet.add(NextNode))  
        					WorkList.add(NextNode);
        			}	
        	}
        }
        
        /* Getting here means there is no circle in the graph
         * Then need to check if the size is consistent:
         * walk through all the points in the graph without repeat
         */
        
        WorkList.clear();
    	VisitedSet.clear(); 
    	
        for(int i=0;i<AllNodes.length;i++) {
        	
        	// if not visited, save in the WorkList
        	if(VisitedSet.add(AllNodes[i]))
        		WorkList.add(AllNodes[i]);
        	
        	while(!WorkList.isEmpty()) {
        		CurrentNode=WorkList.removeFirst();
        		for(int j=0;j<CurrentNode.AdjList.length;j++) {
        			NextNode=CurrentNode.AdjList[j];
        			
        			// if not visited, save in the WorkList
        			if(VisitedSet.add(NextNode))
        				WorkList.add(NextNode);
        		}
        	}
        }
        
        return (VisitedSet.size()==size);  // check if the size is consistent
    }

    public static IFinitization finAcyclicGraph(int size) {
        return finAcyclicGraph(size,size, size);
    }

    public static IFinitization finAcyclicGraph(int nodenum,int minSize,int maxSize) {
        IFinitization f = FinitizationFactory.create(AcyclicGraph.class);
        IObjSet nodes=f.createObjSet(Node.class,true);
        IIntSet arraylength=f.createIntSet(minSize, maxSize);
        IIntSet adjlength=f.createIntSet(0, nodenum-1);
        IArraySet allnodes=f.createArraySet(Node[].class,arraylength,nodes,1);
        IArraySet adjlist=f.createArraySet(Node[].class,adjlength,nodes,1);
        f.set("size", f.createIntSet(minSize, maxSize));
        f.set("AllNodes",allnodes);
        f.set("AdjList", adjlist);
        return f;
    }
}
