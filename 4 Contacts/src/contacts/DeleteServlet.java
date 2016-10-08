package contacts;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;


@WebServlet (name = "DeleteServlet")
public class DeleteServlet extends HttpServlet
{
    private String stylesheet = "edit-style.css";

    //language=HTML
    private String title =
            "<header>Delete Contact</header>";

    //language=HTML
    private String deleteForm =
            "<form method='post' class='edit'>" +
                    "<input type='text' name='id-field' placeholder='ID' required "
                    + ServletsHelper.digitsPattern + " title='Enter a valid ID'>" +
                    "<input type='submit' value='Delete'>"+
            "</form>";

    protected void doPost (HttpServletRequest request, HttpServletResponse response) throws
                                                                                     ServletException,
                                                                                     IOException {
        String errorDescription = DBConnectivity.getInstance().Delete(request.getParameter("id-field"));

        String confirmation = ServletsHelper.generateConfirmation(errorDescription,
                "Successfully deleted");

        ServletsHelper.writeDocument(response,
                new String[]{stylesheet},

                title +
                        deleteForm +
                        confirmation);
    }

    protected void doGet (HttpServletRequest request, HttpServletResponse response) throws
                                                                                    ServletException,
                                                                                    IOException {
        ServletsHelper.writeDocument(response,
                new String[]{stylesheet},
                title + deleteForm);
    }
}
