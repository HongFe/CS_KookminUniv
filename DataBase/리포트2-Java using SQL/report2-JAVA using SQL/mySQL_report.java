/*************************************************************************
 * 																		 *
 *  DB MySQL Report 2													 *
 * 													 					 *
 * 						 컴퓨터 공학과 3학년 20093284 나홍철						 *
 *  																	 *
**************************************************************************/
package homework;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.text.DecimalFormat;

public class HomeWork {
	
	public static void main(String[] args)
	{
		String url = "jdbc:mysql://127.0.0.1:3306/nhcdb";
		Connection conn = null;
		Statement stmt = null;
		Boolean branch_flag = false;
		String branch_name = "";
		
		try {
			Class.forName("java.sql.Driver");
			conn = DriverManager.getConnection(url,"root","root");
			System.out.println("디비 접속 성공");
			stmt = conn.createStatement();

			/**
			 *  branch테이블의 branch_name에 속해있고  속하지 않더라도 branch_name은 출력되도록 한다.
			 *  deposit과 client에 ssn을 비교하여 두곳에 존재하는 고객들만 deposit에서 branch_name에 등록된 해당 지점은행에 속하도록 한다.
			 *  */
			ResultSet rs = stmt.executeQuery("SELECT branch.branch_name , client.name, SUM(deposit.balance)"
					+ "	FROM branch	LEFT JOIN deposit ON branch.branch_name = deposit.branch_name"
					+ " LEFT JOIN client ON client.ssn = deposit.ssn GROUP BY  branch.branch_name,deposit.ssn ORDER BY deposit.branch_name DESC , client.name");
			
			while(rs.next())
			{				
				if(!branch_name.equals("") && !rs.getString(1).equals(branch_name))
					branch_flag=true;
				else
					branch_flag=false;
				
				if(branch_name.equals("") || branch_flag)
				{
					branch_name = rs.getString(1);
					System.out.println("\n" + branch_name );
					System.out.println("+-----+------------+");
					System.out.println("| 고객명 |    예금총액      |");
					System.out.println("+-----+------------+");
				}	
				
				
				if(rs.getString(2)==null || rs.getString(3)==null)
					continue;
				
				System.out.printf( "|%4s | %10s |\n", rs.getString(2), MoneyComma(rs.getString(3)) );
				System.out.println("+-----+------------+");				
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		finally
		{	
			try{
				conn.close();
			}
			catch(Exception ignored){
			}
			try{
				stmt.close();
			}
			catch(Exception ignored){
			}
		}
	}
	
	/**
	 * 
	 * @param balance
	 * @return
	 */
	static String MoneyComma(String balance){
		
		String moneyStr = "";
		int money = Integer.parseInt(balance);
		DecimalFormat myFormatter = new DecimalFormat("#,###");
		moneyStr = myFormatter.format(money);
		 
		return moneyStr;		
	}
}
