package com.byreytiz.launcher.other;

import com.byreytiz.game.gui.models.Actions;
import com.byreytiz.launcher.model.News;
import com.byreytiz.launcher.model.Servers;

import java.util.List;

import retrofit2.Call;
import retrofit2.http.GET;

public interface Interface {

    @GET("http://wh3606.web3.maze-host.ru/zakazi/35/servers.json")
    Call<List<Servers>> getServers();

    @GET("http://wh3606.web3.maze-host.ru/zakazi/35/stories.json")
    Call<List<News>> getNews();

    @GET("http://wh3606.web3.maze-host.ru/donate.json")
    Call<List<Actions>> getActions();
}
