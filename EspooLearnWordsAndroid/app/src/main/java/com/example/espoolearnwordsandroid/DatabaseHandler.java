package com.example.espoolearnwordsandroid;

import android.content.ContentValues;
import android.content.Context;

import android.database.Cursor;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteDatabase;

import java.util.ArrayList;
import java.util.HashMap;

public class DatabaseHandler extends SQLiteOpenHelper
{
    private static final int DB_VERSION = 1;
    private static final String DB_NAME = "dictionarydb";
    private static final String TABLE_Words = "worddetails";
    private static final String KEY_ID = "id";
    public static final String KEY_POLISH = "polish_word";
    public static final String KEY_ENGLISH = "english_word";
    public static final String KEY_PRONUNCIATION = "pronunciation";

    public DatabaseHandler(Context context)
    {
        super(context,DB_NAME, null, DB_VERSION);
    }
    @Override
    public void onCreate(SQLiteDatabase db){
        String CREATE_TABLE = "CREATE TABLE " + TABLE_Words + "("
                + KEY_ID + " INTEGER PRIMARY KEY AUTOINCREMENT," + KEY_POLISH + " TEXT,"
                + KEY_ENGLISH + " TEXT,"
                + KEY_PRONUNCIATION + " TEXT"+ ")";
        db.execSQL(CREATE_TABLE);
    }
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion){
        // Drop older table if exist
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_Words);
        // Create tables again
        onCreate(db);
    }
    // **** CRUD (Create, Read, Update, Delete) Operations ***** //

    // Adding new User Details
    void insertTranslation(String polWord, String engWord, String pronun){
        //Get the Data Repository in write mode
        SQLiteDatabase db = this.getWritableDatabase();
        //Create a new map of values, where column names are the keys
        ContentValues cValues = new ContentValues();
        cValues.put(KEY_POLISH, polWord);
        cValues.put(KEY_ENGLISH, engWord);
        cValues.put(KEY_PRONUNCIATION, pronun);
        // Insert the new row, returning the primary key value of the new row
        long newRowId = db.insert(TABLE_Words,null, cValues);
        db.close();
    }
    // Get User Details
    public ArrayList<HashMap<String, String>> GetWords()
    {
        SQLiteDatabase db = this.getWritableDatabase();
        ArrayList<HashMap<String, String>> wordList = new ArrayList<>();

        String query = "SELECT " + KEY_POLISH + ", " + KEY_ENGLISH + ", " + KEY_PRONUNCIATION + " FROM " + TABLE_Words;
        Cursor cursor = db.rawQuery(query,null);

        while (cursor.moveToNext())
        {
            HashMap<String,String> word = new HashMap<>();
            word.put(KEY_POLISH,cursor.getString(cursor.getColumnIndex(KEY_POLISH)));
            word.put(KEY_ENGLISH,cursor.getString(cursor.getColumnIndex(KEY_ENGLISH)));
            word.put(KEY_PRONUNCIATION,cursor.getString(cursor.getColumnIndex(KEY_PRONUNCIATION)));

            wordList.add(word);
        }
        return  wordList;
    }
    // Get User Details based on wordid
    public ArrayList<HashMap<String, String>> GetWordByWordId(int wordid)
    {
        SQLiteDatabase db = this.getWritableDatabase();
        ArrayList<HashMap<String, String>> wordList = new ArrayList<>();
        String query = "SELECT " + KEY_POLISH + ", " + KEY_ENGLISH + ", " + KEY_PRONUNCIATION + " FROM "+ TABLE_Words;
        Cursor cursor = db.query(TABLE_Words, new String[]{KEY_POLISH, KEY_ENGLISH, KEY_PRONUNCIATION}, KEY_ID+ "=?",new String[]{String.valueOf(wordid)},null, null, null, null);
        if (cursor.moveToNext())
        {
            HashMap<String,String> word = new HashMap<>();
            word.put(KEY_POLISH,cursor.getString(cursor.getColumnIndex(KEY_POLISH)));
            word.put(KEY_ENGLISH,cursor.getString(cursor.getColumnIndex(KEY_ENGLISH)));
            word.put(KEY_PRONUNCIATION,cursor.getString(cursor.getColumnIndex(KEY_PRONUNCIATION)));
            wordList.add(word);
        }
        return  wordList;
    }
    // Delete User Details
    public void DeleteWord(int wordid){
        SQLiteDatabase db = this.getWritableDatabase();
        db.delete(TABLE_Words, KEY_ID+" = ?",new String[]{String.valueOf(wordid)});
        db.close();
    }
    // Update Word Details
    public int UpdateWordDetails(String eng_word, String pronun, int id){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues cVals = new ContentValues();
        cVals.put(KEY_ENGLISH, eng_word);
        cVals.put(KEY_PRONUNCIATION, pronun);
        int count = db.update(TABLE_Words, cVals, KEY_ID+" = ?",new String[]{String.valueOf(id)});
        return  count;
    }
}
