Program # 4

Name:Sam Blair
Cosc 4730

Description: I used Samsung Galaxy S9 on android pie (api 28 i think)

Anything that doesn't work:I think everything workd


# Graded: 48/50 #

* Invalid id input crashes the application. **(-2 points)**

Requiring user input for edits/deletions is not a very good way of handling this functionality (especially given that if they make a mistake, it will cause their app to crash). A better way would be to utilize the swipe/touch even listeners (which mobile devices are so great for) on the items within the RecyclerView themself.
